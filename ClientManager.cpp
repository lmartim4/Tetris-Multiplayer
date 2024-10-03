#include "ClientManager.hpp"
#include "PacketType.hpp"
#include <iostream>

// Constructor
ClientManager::ClientManager(const std::string &serverAddress, uint16_t port)
{
    host = enet_host_create(nullptr, 1, 2, 0, 0); // 1 client, 2 channels

    if (!host)
        throw std::runtime_error("Failed to create ENet client host.");

    ENetAddress address;

    enet_address_set_host(&address, serverAddress.c_str());
    address.port = port;

    peer = enet_host_connect(host, &address, 2, 0);
    if (!peer)
        throw std::runtime_error("No available peers for initiating an ENet connection.");

    actionThread = std::thread(&ClientManager::actionLoop, this); // Start the action loop
}

// Destructor
ClientManager::~ClientManager()
{
    enet_host_destroy(host);

    if (networkThread.joinable())
        networkThread.join();
    if (actionThread.joinable())
        actionThread.join();
}

// Function that runs every 2 seconds to send random command
void ClientManager::actionLoop()
{
    while (isRunning())
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        send_packet(Packet(PacketType::JOIN_REQUEST, {}, peer)); // Set destination as the server peer
    }
}
