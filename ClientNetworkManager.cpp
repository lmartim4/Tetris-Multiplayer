// ClientNetworkManager.cpp

#include "ClientNetworkManager.hpp"
#include "TetrisActions.hpp"
#include <iostream>

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
    actionThread = std::thread(&ClientNetworkManager::actionLoop, this);

    // Seed random number generator
    std::srand(std::time(0));
}

// Destructor
ClientNetworkManager::~ClientNetworkManager() {
    enet_host_destroy(client);
    if (networkThread.joinable()) {
        networkThread.join();
    }
    if (actionThread.joinable()) {
        actionThread.join();
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

// Function to send a random Tetris action
void ClientNetworkManager::sendRandomAction() {
    // Define possible actions
    TetrisAction actions[] = {
        TetrisAction::START, TetrisAction::RESTART, TetrisAction::PAUSE,
        TetrisAction::RESUME, TetrisAction::LEFT, TetrisAction::RIGHT,
        TetrisAction::ROTATE_LEFT, TetrisAction::ROTATE_RIGHT,
        TetrisAction::DROP_FASTER, TetrisAction::DROP_INSTANT
    };

    // Select a random action
    int randomIndex = std::rand() % 10;
    TetrisAction action = actions[randomIndex];

    // Create a packet to send the action
    std::vector<uint8_t> data = { static_cast<uint8_t>(action) };
    Packet actionPacket(data);

    // Send the packet
    enqueueOutgoingPacket(actionPacket);

    // Print the sent action
    std::cout << "Sent action: " << randomIndex << std::endl;
}

// Function that runs every 2 seconds to send random action
void ClientNetworkManager::actionLoop() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        sendRandomAction();
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
