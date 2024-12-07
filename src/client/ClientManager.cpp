#include "ClientManager.hpp"
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "PlayerData.hpp"

void ClientManager::onPeerConnect(ENetPeer *peer)
{
    isConnected = true;
    std::cout << std::endl;
    network_print("");
    std::cout << "Connection successful to " << uint32_to_ipv4(peer->address.host) << ":" << peer->address.port << std::endl;
}
void ClientManager::onPeerDisconnect(ENetPeer *peer)
{
    isConnected = false;
    network_print("Disconnected from server\n");
    disconnect();
}

void ClientManager::on_receive_heartbeat()
{
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    last_heartbeat = now_ms;
}

void ClientManager::on_receive_player_list(const Packet &packet)
{
    players.clear();

    nlohmann::json received;

    try
    {
        received = packet.toJson();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to parse packet as JSON: " << e.what() << std::endl;
        return;
    }

    if (!received.is_array())
    {
        std::cerr << "Invalid JSON: top-level element is not an array." << std::endl;
        return;
    }

    // Iterate over each inner array
    for (const auto &innerArray : received)
    {
        if (!innerArray.is_array())
        {
            std::cerr << "Inner element is not an array. Skipping." << std::endl;
            continue;
        }

        // Iterate over each player object in the inner arrays
        for (const auto &playerObject : innerArray)
        {
            if (!playerObject.is_object())
            {
                std::cerr << "Player entry is not a valid JSON object. Skipping." << std::endl;
                continue;
            }

            PlayerData playerData;

            if (playerObject.contains("isConnected") && playerObject["isConnected"].is_boolean())
                playerData.isConnected = playerObject["isConnected"].get<bool>();

            if (playerObject.contains("playerID") && playerObject["playerID"].is_number_integer())
                playerData.playerID = playerObject["playerID"].get<int>();

            if (playerObject.contains("playerName") && playerObject["playerName"].is_string())
                playerData.playerName = playerObject["playerName"].get<std::string>();

            if (playerObject.contains("score") && playerObject["score"].is_number_integer())
                playerData.score = playerObject["score"].get<int>();

            players.emplace_back(playerData);
        }
    }
}

void ClientManager::TaskStartHeartbeat()
{
    network_print("Initializing Heartbeat Task...\n");
    ThreadHeartbeat = std::thread(&ClientManager::TaskHeartbeat, this);
}

void ClientManager::TaskHeartbeat()
{
    HeartBeatRunningFlag = true;
    int heartbeat_frequencie = 1; // Hertz
    while (HeartBeatRunningFlag)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / heartbeat_frequencie));
        send_packet(Packet(PacketType::HEARTBEAT, 0, serverPeer)); // Set destination as the server serverPeer
    }
}

void ClientManager::toggleDebug()
{
    bool oldState = debugEnabled;
    debugEnabled = !debugEnabled;
    network_print(("Debug mode from " + std::string(oldState ? "Enabled" : "Disabled") +
                   " to " + std::string(debugEnabled ? "Enabled" : "Disabled") + "\n")
                      .c_str());
}

void ClientManager::TaskStopHeartbeat()
{
    if (!HeartBeatRunningFlag)
    {
        std::cout << "Heartbeat was not running. Why would you stop it?\n";
        return;
    }
    HeartBeatRunningFlag = false;

    if (ThreadHeartbeat.joinable())
        ThreadHeartbeat.join();
}

void ClientManager::connect(const std::string &serverAddress, uint16_t port)
{
    std::cout << std::endl;
    std::cout << "╭───────────────────────────────────────╮" << std::endl;
    std::cout << "│          Starting Tetris Client       │" << std::endl;
    std::cout << "├───────────────────────────────────────┤" << std::endl;
    std::cout << "│   Using NetworkManager version v" << NetworkManager::version << "     │" << std::endl;
    std::cout << "│   Connection target: " << serverAddress << ":" << port << "  │" << std::endl;
    std::cout << "╰───────────────────────────────────────╯" << std::endl;
    std::cout << std::endl;

    host = enet_host_create(nullptr, 1, 2, 0, 0); // 1 client, 2 channels

    try
    {
        if (!host)
            throw std::runtime_error("Failed to create ENet client host.");
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return;
    }

    ENetAddress address;
    enet_address_set_host(&address, serverAddress.c_str());
    address.port = port;

    serverPeer = enet_host_connect(host, &address, 2, 0);

    try
    {
        if (!serverPeer)
            throw std::runtime_error("No available serverPeers for initiating an ENet connection.");
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return;
    }

    TaskStartHeartbeat();
    TaskStartNetwork();
}

void ClientManager::disconnect()
{
    if (serverPeer)
    {
        enet_peer_disconnect_now(serverPeer, 0);
        enet_peer_reset(serverPeer);
    }
    
    TaskStopNetwork();
    TaskStopHeartbeat();
}

void ClientManager::on_receive_game_screen(const Packet &packet)
{
    std::cout << "Received a board\n";
    try
    {
        nlohmann::json boardData = packet.toJson();
        std::lock_guard<std::mutex> lock(boardMutex);
        lastReceivedBoard = boardData;
        hasBoardData = true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to parse game screen board JSON: " << e.what() << std::endl;
    }
    return;
}

nlohmann::json ClientManager::getLastBoardState()
{
    std::lock_guard<std::mutex> lock(boardMutex);
    return lastReceivedBoard;
}

// SE os quadros desincronizarem pode estar aqui o problema
bool ClientManager::boardDataAvailable()
{
    std::lock_guard<std::mutex> lock(boardMutex);
    if (hasBoardData)
    {
        hasBoardData = false;
        return true;
    }
    return false;
}

void ClientManager::request_game_start()
{
    send_packet(Packet(PacketType::REQUEST_START, 0, serverPeer));
}

void ClientManager::onPressKey(sf::Event::KeyEvent e){
    switch (e.code)
    {
    case sf::Keyboard::P:
        //if (debugEnabled) clientManager.network_print("Tecla P pressionada. Pausar o jogo.\n");
        send_packet(Packet(PacketType::PAUSE, 0, serverPeer));
        break;

    case sf::Keyboard::R:
        if (debugEnabled)
            network_print("Tecla R pressionada. Reiniciar o jogo.\n");
        send_packet(Packet(PacketType::RESTART, 0, serverPeer));
        break;
    case sf::Keyboard::Up:
        if (debugEnabled)
            network_print("Tecla Cima (Up) pressionada.\n");
        send_packet(Packet(PacketType::ROTATE_RIGHT, 0, serverPeer));
        break;

    case sf::Keyboard::Down:
        if (debugEnabled)
            network_print("Tecla Baixo (Down) pressionada.\n");
        send_packet(Packet(PacketType::DROP_FASTER, 0, serverPeer));
        break;

    case sf::Keyboard::Left:
        if (debugEnabled)
            network_print("Tecla Esquerda (Left) pressionada.\n");
        send_packet(Packet(PacketType::LEFT, 0, serverPeer));
        break;

    case sf::Keyboard::Right:
        if (debugEnabled)
            network_print("Tecla Direita (Right) pressionada.\n");
        send_packet(Packet(PacketType::RIGHT, 0, serverPeer));
        break;

    case sf::Keyboard::W:
        if (debugEnabled)
            network_print("Tecla W pressionada.\n");
        send_packet(Packet(PacketType::ROTATE_LEFT, 0, serverPeer));
        break;

    case sf::Keyboard::A:
        if (debugEnabled)
            network_print("Tecla A pressionada.\n");
        send_packet(Packet(PacketType::LEFT, 0, serverPeer));
        break;

    case sf::Keyboard::S:
        if (debugEnabled)
            network_print("Tecla S pressionada.\n");
        send_packet(Packet(PacketType::DROP_INSTANT, 0, serverPeer));
        break;

    case sf::Keyboard::D:
        if (debugEnabled)
            network_print("Tecla D pressionada.\n");
        send_packet(Packet(PacketType::RIGHT, 0, serverPeer));
        break;

    case sf::Keyboard::G:
        toggleDebug();
        break;

    default:
        break;
    }
}