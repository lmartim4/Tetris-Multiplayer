#include "ClientNetworkManager.hpp"
#include "PacketType.hpp"
#include <iostream>

// Constructor
ClientNetworkManager::ClientNetworkManager(const std::string& serverAddress, uint16_t port) {
    host = enet_host_create(nullptr, 1, 2, 0, 0);  // 1 client, 2 channels
    if (!host) {
        throw std::runtime_error("Failed to create ENet client host.");
    }

    ENetAddress address;
    enet_address_set_host(&address, serverAddress.c_str());
    address.port = port;

    peer = enet_host_connect(host, &address, 2, 0);
    if (!peer) {
        throw std::runtime_error("No available peers for initiating an ENet connection.");
    }

    networkThread = std::thread(&NetworkManager::networkLoop, this);  // Start the network loop
    actionThread = std::thread(&ClientNetworkManager::actionLoop, this);  // Start the action loop

    // Seed random number generator
    std::srand(std::time(0));
}

// Destructor
ClientNetworkManager::~ClientNetworkManager() {
    enet_host_destroy(host);
    if (networkThread.joinable()) {
        networkThread.join();
    }
    if (actionThread.joinable()) {
        actionThread.join();
    }
}

// Process ENet events for the client
void ClientNetworkManager::processENetEvent(ENetEvent& event) {
    switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
            std::cout << "Connected to server." << std::endl;
            break;

        case ENET_EVENT_TYPE_RECEIVE:
            {
                Packet packet = parsePacket(event.packet, event.peer);  // Parse packet with source peer
                handlePacket(packet, event.peer);  // Handle the received packet
                enet_packet_destroy(event.packet);
            }
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            std::cout << "Disconnected from server." << std::endl;
            peer = nullptr;
            break;

        default:
            break;
    }
}

// Function that runs every 2 seconds to send random command
void ClientNetworkManager::actionLoop() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Send a random command to the server
        PacketType commands[] = {
            PacketType::JOIN_REQUEST, PacketType::START, PacketType::LEFT,
            PacketType::RIGHT, PacketType::ROTATE_LEFT, PacketType::ROTATE_RIGHT,
            PacketType::DROP_FASTER, PacketType::DROP_INSTANT
        };
        int randomIndex = std::rand() % 8;

        // Enqueue a packet to send to the server
        enqueueOutgoingPacket(Packet(commands[0], {}, nullptr, peer));  // Set destination as the server peer
    }
}
