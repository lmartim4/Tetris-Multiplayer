#include "NetworkManager.hpp"

// Constructor
NetworkManager::NetworkManager() : running(true) {}

// Destructor
NetworkManager::~NetworkManager() {
    stop();
}

// Enqueue an outgoing packet
void NetworkManager::enqueueOutgoingPacket(const Packet &packet) {
    outgoingPackets.push(packet);
}

// Register a listener for a specific packet type
void NetworkManager::registerListener(PacketType packetType, std::function<void(const std::vector<uint8_t> &)> callback) {
    listeners[static_cast<uint8_t>(packetType)] = callback;
}

// Handle a received packet by triggering the corresponding listener
void NetworkManager::handlePacket(const Packet &packet) {
    uint8_t packetType = static_cast<uint8_t>(packet.type);
    if (listeners.find(packetType) != listeners.end()) {
        listeners[packetType](packet.data);
    }
}

// Process incoming packets
void NetworkManager::processIncomingPackets() {
    while (!incomingPackets.empty()) {
        Packet packet = incomingPackets.front();
        incomingPackets.pop();
        handlePacket(packet);
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

// Common method: Parse an incoming ENet packet into a custom Packet structure
Packet NetworkManager::parsePacket(ENetPacket *enetPacket) {
    if (enetPacket->dataLength < 1) return Packet(PacketType::CUSTOM, {});
    PacketType type = static_cast<PacketType>(enetPacket->data[0]); // First byte is the packet type
    std::vector<uint8_t> data(enetPacket->data + 1, enetPacket->data + enetPacket->dataLength); // Remaining data
    return Packet(type, data);
}

// Common method: Create an ENet packet from a custom Packet structure
ENetPacket *NetworkManager::createENetPacket(const Packet &packet) {
    std::vector<uint8_t> rawData = packet.toRawData();
    return enet_packet_create(rawData.data(), rawData.size(), ENET_PACKET_FLAG_RELIABLE);
}
