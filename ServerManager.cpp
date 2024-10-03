#include "ServerManager.hpp"
#include <iostream>
#include <stdexcept>

ServerManager::ServerManager(uint16_t port)
{
    std::cout << "============================" << std::endl;
    std::cout << "Starting Server Network..." << std::endl;
    std::cout << "Using NetworkManager version v" << NetworkManager::version << std::endl;
    std::cout << "Opening server at port " << port << std::endl;
    std::cout << "============================" << std::endl
              << std::endl;

    if (enet_initialize() != 0)
    {
        std::cerr << "Failed to initialize server!" << std::endl;
        throw std::runtime_error("Failed to initialize ENet.");
    }

    ENetAddress address;
    enet_address_set_host(&address, "0.0.0.0");
    address.port = port;

    host = enet_host_create(&address, 32, 2, 0, 0); // 32 clients max, 2 channels
    if (!host)
        throw std::runtime_error("Failed to create ENet server host.");
}

ServerManager::~ServerManager()
{
    enet_host_destroy(host);
    if (networkThread.joinable())
        networkThread.join();
}