// ClientNetworkManager.cpp

#include "ClientNetworkManager.hpp"
#include <iostream>
#include <enet/enet.h>

// Constructor
ClientNetworkManager::ClientNetworkManager(const std::string& serverAddress, uint16_t port) {
    client = enet_host_create(nullptr, 1, 2, 0, 0);  // 1 client, 2 channels
    if (!client) {
        throw std::runtime_error("Failed to create ENet client host.");
    }

    ENetAddress address;
    enet_address_set_host(&address, serverAddress.c_str());
    address.port = port;

    peer = enet_host_connect(client, &address, 2, 0);
    if (!peer) {
        throw std::runtime_error("No available peers for initiating an ENet connection.");
    }

    networkThread = std::thread(&ClientNetworkManager::networkLoop, this);
    heartbeatThread = std::thread(&ClientNetworkManager::sendHeartbeatLoop, this);
}

// Destructor
ClientNetworkManager::~ClientNetworkManager() {
    enet_host_destroy(client);
    if (networkThread.joinable()) {
        networkThread.join();
    }
    if (heartbeatThread.joinable()) {
        heartbeatThread.join();
    }
}

// Main network loop
void ClientNetworkManager::networkLoop() {
    while (running) {
        ENetEvent event;
        while (enet_host_service(client, &event, 1000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "Connected to server." << std::endl;
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "Packet received from server." << std::endl;
                enqueueIncomingPacket(parsePacket(event.packet));
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnected from server." << std::endl;
                peer = nullptr;
                break;

            default:
                break;
            }
        }

        sendOutgoingPackets();
        processIncomingPackets();
    }
}

// Send heartbeat packet every second
void ClientNetworkManager::sendHeartbeatLoop() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Create and send the heartbeat packet
        std::vector<uint8_t> data = {'H', 'E', 'A', 'R', 'T', 'B', 'E', 'A', 'T'};
        Packet heartbeatPacket(data);
        enqueueOutgoingPacket(heartbeatPacket);
    }
}

// Send all outgoing packets
void ClientNetworkManager::sendOutgoingPackets() {
    while (!outgoingPackets.empty()) {
        Packet packet = outgoingPackets.front();
        outgoingPackets.pop();

        ENetPacket* enetPacket = createENetPacket(packet);
        enet_peer_send(peer, 0, enetPacket);
    }
}

// Parse an incoming ENet packet into a custom Packet structure
Packet ClientNetworkManager::parsePacket(ENetPacket* enetPacket) {
    std::vector<uint8_t> data(enetPacket->data, enetPacket->data + enetPacket->dataLength);
    return Packet(data);
}

// Create an ENet packet from a custom Packet structure
ENetPacket* ClientNetworkManager::createENetPacket(const Packet& packet) {
    return enet_packet_create(packet.data.data(), packet.data.size(), ENET_PACKET_FLAG_RELIABLE);
}
