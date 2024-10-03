#include "ServerNetworkManager.hpp"
#include <iostream>
#include <stdexcept>

// Constructor
ServerNetworkManager::ServerNetworkManager(uint16_t port) {
    ENetAddress address;
    enet_address_set_host(&address, "0.0.0.0");
    address.port = port;

    host = enet_host_create(&address, 32, 2, 0, 0);  // 32 clients max, 2 channels
    if (!host) {
        throw std::runtime_error("Failed to create ENet server host.");
    }

    networkThread = std::thread(&NetworkManager::networkLoop, this);  // Start the network loop
}

// Destructor
ServerNetworkManager::~ServerNetworkManager() {
    enet_host_destroy(host);
    if (networkThread.joinable()) {
        networkThread.join();
    }
}

// Process ENet events for the server
void ServerNetworkManager::processENetEvent(ENetEvent& event) {
    switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
            std::cout << "Client connected." << std::endl;
            break;

        case ENET_EVENT_TYPE_RECEIVE:
            {
                Packet packet = parsePacket(event.packet, event.peer);  // Parse packet with source peer
                handlePacket(packet, event.peer);  // Handle the received packet
                enet_packet_destroy(event.packet);
            }
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            std::cout << "Client disconnected." << std::endl;
            break;

        default:
            break;
    }
}
