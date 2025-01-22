#include "ClientManager.hpp"
#include "Clock.hpp"
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "PlayerData.hpp"
#include "SoundType.hpp"

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

void ClientManager::on_receive_score(const Packet &packet)
{
    //std::cout << packet.getPayloadAsJson() << std::endl;
    score.deserialize(packet.getPayloadAsJson());
}

void ClientManager::on_receive_heartbeat()
{
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    last_heartbeat = now_ms;
}

void ClientManager::on_receive_player_list(const Packet &packet)
{
    players.deserialize(packet.getPayloadAsJson());
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
        sendPacket(Packet(PacketType::HEARTBEAT, serverPeer)); // Set destination as the server serverPeer
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
    try
    {
        nlohmann::json boardData = packet.getPayloadAsJson();
        // boardBuffer.clear();
        boardBuffer.push(boardData);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to parse game screen board JSON: " << e.what() << std::endl;
    }
}

void ClientManager::on_receive_end_screen(const Packet &packet)
{
    endGameDataBuffer.push(packet.getPayloadAsJson());
}

void ClientManager::on_receive_play_sound(const Packet &packet)
{

    audio.playSound((SoundType)packet.getData()[0]);
}

bool ClientManager::hasBoard(nlohmann::json &board)
{
    return boardBuffer.pop(board);
}

bool ClientManager::hasEndGameData(nlohmann::json &endGame)
{
    return endGameDataBuffer.pop(endGame);
}

void ClientManager::request_game_start()
{
    sendPacket(Packet(PacketType::REQUEST_START, serverPeer));
}

void ClientManager::onPressKey(sf::Event::KeyEvent e)
{
    switch (e.code)
    {
    case sf::Keyboard::P:
        if (debugEnabled)
            network_print("Tecla P pressionada. Pausar o jogo.\n");
        sendPacket(Packet(PacketType::PAUSE, serverPeer));
        break;

    case sf::Keyboard::R:
        if (debugEnabled)
            network_print("Tecla R pressionada. Reiniciar o jogo.\n");
        sendPacket(Packet(PacketType::RESTART, serverPeer));
        break;
    case sf::Keyboard::Up:
        if (debugEnabled)
            network_print("Tecla Cima (Up) pressionada.\n");
        sendPacket(Packet(PacketType::ROTATE_CW, serverPeer));
        break;

    case sf::Keyboard::Down:
        if (debugEnabled)
            network_print("Tecla Baixo (Down) pressionada.\n");
        sendPacket(Packet(PacketType::ROTATE_CCW, serverPeer));
        break;

    case sf::Keyboard::Left:
        if (debugEnabled)
            network_print("Tecla Esquerda (Left) pressionada.\n");
        sendPacket(Packet(PacketType::LEFT, serverPeer));
        break;

    case sf::Keyboard::Right:
        if (debugEnabled)
            network_print("Tecla Direita (Right) pressionada.\n");
        sendPacket(Packet(PacketType::RIGHT, serverPeer));
        break;

    case sf::Keyboard::Space:
        if (debugEnabled)
            network_print("Tecla Espaco (Space) pressionada.\n");
        sendPacket(Packet(PacketType::HARD_DROP, serverPeer));
        break;

    case sf::Keyboard::W:
        if (debugEnabled)
            network_print("Tecla W pressionada.\n");
        sendPacket(Packet(PacketType::ROTATE_CCW, serverPeer));
        break;

    case sf::Keyboard::A:
        if (debugEnabled)
            network_print("Tecla A pressionada.\n");
        sendPacket(Packet(PacketType::LEFT, serverPeer));
        break;

    case sf::Keyboard::S:
        if (debugEnabled)
            network_print("Tecla S pressionada.\n");
        sendPacket(Packet(PacketType::ROTATE_CW, serverPeer));
        break;

    case sf::Keyboard::D:
        if (debugEnabled)
            network_print("Tecla D pressionada.\n");
        sendPacket(Packet(PacketType::RIGHT, serverPeer));
        break;

    case sf::Keyboard::Enter:
        if (debugEnabled)
            network_print("Tecla Espaco (Enter) pressionada.\n");
        sendPacket(Packet(PacketType::DROP_FASTER, serverPeer));
        break;

    case sf::Keyboard::G:
        toggleDebug();
        break;

    default:
        break;
    }
}