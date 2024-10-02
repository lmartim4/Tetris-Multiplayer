// NetworkManager.cpp

#include "NetworkManager.hpp"

// Constructor
NetworkManager::NetworkManager() : running(true) {}

// Destructor
NetworkManager::~NetworkManager() {
    stop();
}

// Enqueue an incoming packet
void NetworkManager::enqueueIncomingPacket(const Packet& packet) {
    incomingPackets.push(packet);
}

// Enqueue an outgoing packet
void NetworkManager::enqueueOutgoingPacket(const Packet& packet) {
    outgoingPackets.push(packet);
}

// Process incoming packets
void NetworkManager::processIncomingPackets() {
    while (!incomingPackets.empty()) {
        Packet packet = incomingPackets.front();
        incomingPackets.pop();
        // Process the packet here or trigger event handling
        std::cout << "Processing incoming packet" << std::endl;
    }
}

// Stop the network loop
void NetworkManager::stop() {
    running = false;
}

// Check if the network manager is running
bool NetworkManager::isRunning() const {
    return running;
}
