#include "ClientManager.hpp"
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>

ClientManager::ClientManager() {}
ClientManager::~ClientManager() {}

void ClientManager::onPeerConnect(ENetPeer * peer)
{
    isConnected = true;
    std::cout << std::endl;
    network_print("");
    std::cout << "Connection successful to " << uint32_to_ipv4(peer->address.host) << ":" << peer->address.port << std::endl
              << std::endl;
}
void ClientManager::onPeerDisconnect(ENetPeer * peer)
{
    isConnected = false;
    network_print("Disconnected from server\n");
}

void ClientManager::on_receive_heartbeat()
{
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    last_heartbeat = now_ms;
}

void ClientManager::TaskStartHeartbeat()
{
    network_print("Initializing Heartbeat Task...\n");
    ThreadHeartbeat = std::thread(&ClientManager::TaskHeartbeat, this);
}

void ClientManager::TaskHeartbeat()
{
    int heartbeat_frequencie = 1; // Hertz
    while (isRunning())
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

void ClientManager::handleKeyPress(sf::Event keyEvent, sf::RenderWindow &window)
{
    switch (keyEvent.key.code)
    {
    case sf::Keyboard::Escape:
        if (debugEnabled)
            network_print("Tecla ESC pressionada. Fechando a janela.\n");
        window.close();
        break;

    case sf::Keyboard::P:
        if (debugEnabled)
            network_print("Tecla P pressionada. Pausar o jogo.\n");
        send_packet(Packet(PacketType::PAUSE, 0, serverPeer));
        break;

    case sf::Keyboard::R:
        if (debugEnabled)
            network_print("Tecla R pressionada. Reiniciar o jogo.\n");
        send_packet(Packet(PacketType::RESTART, 0, serverPeer));
        break;

    case sf::Keyboard::Q:
        if (debugEnabled)
            network_print("Tecla Q pressionada. Sair do jogo.\n");
        window.close();
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
