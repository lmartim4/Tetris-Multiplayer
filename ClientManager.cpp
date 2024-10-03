#include "ClientManager.hpp"
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>

ClientManager::ClientManager(const std::string &serverAddress, uint16_t port)
{
    std::cout << "============================" << std::endl;
    std::cout << "Starting Client Network..." << std::endl;
    std::cout << "Using NetworkManager version v" << NetworkManager::version << std::endl;
    std::cout << "Connection target: " << serverAddress << ":" << port << std::endl;
    std::cout << "============================" << std::endl << std::endl;
    
    if (enet_initialize() != 0)
    {
        std::cerr << "Failed to initialize Client" << std::endl;
        throw std::runtime_error("Failed to initialize ENet.");
    }

    host = enet_host_create(nullptr, 1, 2, 0, 0); // 1 client, 2 channels

    if (!host)
        throw std::runtime_error("Failed to create ENet client host.");

    ENetAddress address;
    enet_address_set_host(&address, serverAddress.c_str());
    address.port = port;

    serverPeer = enet_host_connect(host, &address, 2, 0);
    if (!serverPeer)
        throw std::runtime_error("No available serverPeers for initiating an ENet connection.");

    // Inicia a thread do heartbeat
    actionThread = std::thread(&ClientManager::HEARTBEAT_TASK, this);

    // Inicia a thread da janela
    running = true;
    windowThread = std::thread(&ClientManager::windowLoop, this);
}

ClientManager::~ClientManager()
{
    enet_host_destroy(host);

    if (networkThread.joinable())
        networkThread.join();

    if (actionThread.joinable())
        actionThread.join();

    stopWindow(); // Para a janela quando o cliente é destruído
}

void ClientManager::HEARTBEAT_TASK()
{
    int heartbeat_frequencie = 1; // Hertz
    while (isRunning())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / heartbeat_frequencie));
        send_packet(Packet(PacketType::HEARTBEAT, {}, serverPeer)); // Set destination as the server serverPeer
    }
}

// Função para alternar o estado de debug
void ClientManager::toggleDebug()
{
    bool oldState = debugEnabled;
    debugEnabled = !debugEnabled;
    network_print(("Debug mode from " + std::string(oldState ? "Enabled" : "Disabled") + 
                   " to " + std::string(debugEnabled ? "Enabled" : "Disabled") + "\n").c_str());
}

// Loop para rodar a janela
void ClientManager::windowLoop()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tetris Client");
    window.setFramerateLimit(60);

    while (running && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Detecta pressionamento de teclas
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        if (debugEnabled) network_print("Tecla ESC pressionada. Fechando a janela.\n");
                        window.close();
                        break;

                    case sf::Keyboard::P:
                        if (debugEnabled) network_print("Tecla P pressionada. Pausar o jogo.\n");
                        send_packet(Packet(PacketType::PAUSE, {0}, serverPeer));
                        break;

                    case sf::Keyboard::R:
                        if (debugEnabled) network_print("Tecla R pressionada. Reiniciar o jogo.\n");
                        send_packet(Packet(PacketType::RESTART, {0}, serverPeer));
                        break;

                    case sf::Keyboard::Q:
                        if (debugEnabled) network_print("Tecla Q pressionada. Sair do jogo.\n");
                        window.close();
                        break;

                    case sf::Keyboard::Up:
                        if (debugEnabled) network_print("Tecla Cima (Up) pressionada.\n");
                        send_packet(Packet(PacketType::ROTATE_RIGHT, {0}, serverPeer));
                        break;

                    case sf::Keyboard::Down:
                        if (debugEnabled) network_print("Tecla Baixo (Down) pressionada.\n");
                        send_packet(Packet(PacketType::DROP_FASTER, {0}, serverPeer));
                        break;

                    case sf::Keyboard::Left:
                        if (debugEnabled) network_print("Tecla Esquerda (Left) pressionada.\n");
                        send_packet(Packet(PacketType::LEFT, {0}, serverPeer));
                        break;

                    case sf::Keyboard::Right:
                        if (debugEnabled) network_print("Tecla Direita (Right) pressionada.\n");
                        send_packet(Packet(PacketType::RIGHT, {0}, serverPeer));
                        break;

                    case sf::Keyboard::W:
                        if (debugEnabled) network_print("Tecla W pressionada.\n");
                        send_packet(Packet(PacketType::ROTATE_LEFT, {0}, serverPeer));
                        break;

                    case sf::Keyboard::A:
                        if (debugEnabled) network_print("Tecla A pressionada.\n");
                        send_packet(Packet(PacketType::LEFT, {0}, serverPeer));
                        break;

                    case sf::Keyboard::S:
                        if (debugEnabled) network_print("Tecla S pressionada.\n");
                        send_packet(Packet(PacketType::DROP_INSTANT, {0}, serverPeer));
                        break;

                    case sf::Keyboard::D:
                        if (debugEnabled) network_print("Tecla D pressionada.\n");
                        send_packet(Packet(PacketType::RIGHT, {0}, serverPeer));
                        break;

                    case sf::Keyboard::G:
                        toggleDebug();
                        break;

                    default:
                        break;
                }
            }
        }

        window.clear();
        window.display();
    }

    running = false;
}

// Função para parar o loop da janela
void ClientManager::stopWindow()
{
    running = false;
    if (windowThread.joinable())
        windowThread.join();
}
