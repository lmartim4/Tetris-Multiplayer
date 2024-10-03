#include "ServerManager.hpp"
#include <iostream>
#include <stdexcept>

// Constructor
ServerManager::ServerManager(uint16_t port)
{
    ENetAddress address;
    enet_address_set_host(&address, "0.0.0.0");
    address.port = port;

    host = enet_host_create(&address, 32, 2, 0, 0); // 32 clients max, 2 channels
    if (!host)
        throw std::runtime_error("Failed to create ENet server host.");
}

// Destructor
ServerManager::~ServerManager()
{
    enet_host_destroy(host);
    if (networkThread.joinable())
        networkThread.join();
}

