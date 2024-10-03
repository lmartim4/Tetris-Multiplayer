#include "ClientManager.hpp"
#include "PacketType.hpp"
#include <iostream>

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

    peer = enet_host_connect(host, &address, 2, 0);
    if (!peer)
        throw std::runtime_error("No available peers for initiating an ENet connection.");

    actionThread = std::thread(&ClientManager::HEARTBEAT_TASK, this);
}

ClientManager::~ClientManager()
{
    enet_host_destroy(host);

    if (networkThread.joinable())
        networkThread.join();

    if (actionThread.joinable())
        actionThread.join();
}

void ClientManager::HEARTBEAT_TASK()
{
    int heartbeat_frequencie = 1; //Hertz

    while (isRunning())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/heartbeat_frequencie));
        send_packet(Packet(PacketType::HEARTBEAT,{}, peer)); // Set destination as the server peer
    }
}
