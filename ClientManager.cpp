#include "ClientManager.hpp"
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>

void ClientManager::onPeerConnect(ENetPeer *peer)
{
    std::cout << std::endl;
    network_print("");
    std::cout << "Connection successful to " << uint32_to_ipv4(peer->address.host) << ":" << peer->address.port << std::endl
              << std::endl;
}
void ClientManager::onPeerDisconnect(ENetPeer *peer)
{
    network_print("Disconnected from server\n");
}

ClientManager::ClientManager(const std::string &serverAddress, uint16_t port)
{
    std::cout << "============================" << std::endl;
    std::cout << "|| Starting Tetris Client..." << std::endl;
    std::cout << "|| Using NetworkManager version v" << NetworkManager::version << std::endl;
    std::cout << "|| Connection target: " << serverAddress << ":" << port << std::endl;
    std::cout << "============================" << std::endl
              << std::endl;

    host = enet_host_create(nullptr, 1, 2, 0, 0); // 1 client, 2 channels

    if (!host)
        throw std::runtime_error("Failed to create ENet client host.");

    ENetAddress address;
    enet_address_set_host(&address, serverAddress.c_str());
    address.port = port;

    serverPeer = enet_host_connect(host, &address, 2, 0);
    if (!serverPeer)
        throw std::runtime_error("No available serverPeers for initiating an ENet connection.");

    TaskStartHeartbeat();
    TaskStartSFML();
    TaskStartNetwork();
}

ClientManager::~ClientManager()
{
    TaskStopSFML();
    TaskStopHeartbeat();
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
        send_packet(Packet(PacketType::HEARTBEAT, {}, serverPeer)); // Set destination as the server serverPeer
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

void ClientManager::TaskStartSFML()
{
    network_print("Initializing SFML Task...\n");
    sfml_running = true;
    ThreadSFML = std::thread(&ClientManager::TaskSFML, this);
}

void ClientManager::TaskSFML()
{
    sf::RenderWindow window(sf::VideoMode(400, 300), "Tetris Client");
    window.setFramerateLimit(60);

    while (sfml_running && window.isOpen())
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
                    if (debugEnabled)
                        network_print("Tecla ESC pressionada. Fechando a janela.\n");
                    window.close();
                    break;

                case sf::Keyboard::P:
                    if (debugEnabled)
                        network_print("Tecla P pressionada. Pausar o jogo.\n");
                    send_packet(Packet(PacketType::PAUSE, {0}, serverPeer));
                    break;

                case sf::Keyboard::R:
                    if (debugEnabled)
                        network_print("Tecla R pressionada. Reiniciar o jogo.\n");
                    send_packet(Packet(PacketType::RESTART, {0}, serverPeer));
                    break;

                case sf::Keyboard::Q:
                    if (debugEnabled)
                        network_print("Tecla Q pressionada. Sair do jogo.\n");
                    window.close();
                    break;

                case sf::Keyboard::Up:
                    if (debugEnabled)
                        network_print("Tecla Cima (Up) pressionada.\n");
                    send_packet(Packet(PacketType::ROTATE_RIGHT, {0}, serverPeer));
                    break;

                case sf::Keyboard::Down:
                    if (debugEnabled)
                        network_print("Tecla Baixo (Down) pressionada.\n");
                    send_packet(Packet(PacketType::DROP_FASTER, {0}, serverPeer));
                    break;

                case sf::Keyboard::Left:
                    if (debugEnabled)
                        network_print("Tecla Esquerda (Left) pressionada.\n");
                    send_packet(Packet(PacketType::LEFT, {0}, serverPeer));
                    break;

                case sf::Keyboard::Right:
                    if (debugEnabled)
                        network_print("Tecla Direita (Right) pressionada.\n");
                    send_packet(Packet(PacketType::RIGHT, {0}, serverPeer));
                    break;

                case sf::Keyboard::W:
                    if (debugEnabled)
                        network_print("Tecla W pressionada.\n");
                    send_packet(Packet(PacketType::ROTATE_LEFT, {0}, serverPeer));
                    break;

                case sf::Keyboard::A:
                    if (debugEnabled)
                        network_print("Tecla A pressionada.\n");
                    send_packet(Packet(PacketType::LEFT, {0}, serverPeer));
                    break;

                case sf::Keyboard::S:
                    if (debugEnabled)
                        network_print("Tecla S pressionada.\n");
                    send_packet(Packet(PacketType::DROP_INSTANT, {0}, serverPeer));
                    break;

                case sf::Keyboard::D:
                    if (debugEnabled)
                        network_print("Tecla D pressionada.\n");
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

    sfml_running = false;
}

// Função para parar o loop da janela
void ClientManager::TaskStopSFML()
{
    sfml_running = false;
    if (ThreadSFML.joinable())
        ThreadSFML.join();
}

void ClientManager::TaskStopHeartbeat()
{
    if (ThreadHeartbeat.joinable())
        ThreadHeartbeat.join();
}