// ServerNetworkManager.cpp

#include "ServerNetworkManager.hpp"
#include <iostream>
#include <stdexcept>

// Constructor
ServerNetworkManager::ServerNetworkManager(uint16_t port) {
    ENetAddress address;
    enet_address_set_host(&address, "0.0.0.0");
    address.port = port;

    server = enet_host_create(&address, 32, 2, 0, 0);  // 32 clients max, 2 channels
    if (!server) {
        throw std::runtime_error("Failed to create ENet server host.");
    }

    networkThread = std::thread(&ServerNetworkManager::networkLoop, this);
}

// Destructor
ServerNetworkManager::~ServerNetworkManager() {
    enet_host_destroy(server);
    if (networkThread.joinable()) {
        networkThread.join();
    }
}

// Main network loop to process ENet events
void ServerNetworkManager::networkLoop() {
    while (running) {
        ENetEvent event;
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "Client connected: " << event.peer->address.host << std::endl;
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                handlePacket(event);
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Client disconnected." << std::endl;
                break;

            default:
                break;
            }
        }

        sendOutgoingPackets();
        processIncomingPackets();
    }
}

// Send all outgoing packets to connected clients
void ServerNetworkManager::sendOutgoingPackets() {
    while (!outgoingPackets.empty()) {
        Packet packet = outgoingPackets.front();
        outgoingPackets.pop();

        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPacket* enetPacket = createENetPacket(packet);
            enet_peer_send(&server->peers[i], 0, enetPacket);
        }
    }
}

// Parse an incoming ENet packet into a custom Packet structure
Packet ServerNetworkManager::parsePacket(ENetPacket* enetPacket) {
    std::vector<uint8_t> data(enetPacket->data, enetPacket->data + enetPacket->dataLength);
    return Packet(data);
}

// Create an ENet packet from the custom Packet structure
ENetPacket* ServerNetworkManager::createENetPacket(const Packet& packet) {
    return enet_packet_create(packet.data.data(), packet.data.size(), ENET_PACKET_FLAG_RELIABLE);
}

// Handle an incoming packet, for example, to process heartbeat packets
void ServerNetworkManager::handlePacket(const ENetEvent& event) {
    Packet packet = parsePacket(event.packet);

    // Check if the packet is a heartbeat
    std::string packetData(packet.data.begin(), packet.data.end());
    if (packetData == "HEARTBEAT") {
        // Print the client's IP address
        char clientIP[16];
        enet_address_get_host_ip(&event.peer->address, clientIP, sizeof(clientIP));
        std::cout << "Heartbeat received from: " << clientIP << std::endl;
    }
}
