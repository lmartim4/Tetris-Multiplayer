#include "ServerManager.hpp"
#include <iostream>
#include <stdexcept>

#define MAX_CLIENTS 32
#define CHANNELS 2

void ServerManager::onPeerConnect(ENetPeer *peer)
{
    network_print("");
    std::cout << "[CONNECTED] " << uint32_to_ipv4(peer->address.host) << ":" << peer->address.port << std::endl;
}
void ServerManager::onPeerDisconnect(ENetPeer *peer)
{
    network_print("");
    std::cout << "[DISCONNECTED] " << uint32_to_ipv4(peer->address.host) << ":" << peer->address.port << std::endl;
}

ServerManager::ServerManager(uint16_t port)
{
    std::cout << std::endl
              << "===========================" << std::endl;
    std::cout << "|| Starting Tetris Server..." << std::endl;
    std::cout << "|| Using NetworkManager version v" << NetworkManager::version << std::endl;
    std::cout << "|| Opening server at port " << port << std::endl;
    std::cout << "===========================" << std::endl
              << std::endl;

    ENetAddress address;
    enet_address_set_host(&address, "0.0.0.0");
    address.port = port;

    host = enet_host_create(&address, MAX_CLIENTS, CHANNELS, 0, 0);

    if (!host)
        throw std::runtime_error("Failed to create ENet server host.");

    TaskStartNetwork();
}

ServerManager::~ServerManager()
{
}