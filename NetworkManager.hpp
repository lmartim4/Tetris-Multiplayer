#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include "PacketType.hpp"
#include <enet/enet.h>
#include <queue>
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <functional>

// Structure to represent a packet
struct Packet
{
    PacketType type;           // The type of the packet (ACTION, HEARTBEAT, etc.)
    std::vector<uint8_t> data; // The rest of the data in the packet

    Packet(PacketType t, const std::vector<uint8_t> &d)
        : type(t), data(d) {}

    // Serialize the packet for sending over the network
    std::vector<uint8_t> toRawData() const
    {
        std::vector<uint8_t> rawData;
        rawData.push_back(static_cast<uint8_t>(type));           // First byte is the packet type
        rawData.insert(rawData.end(), data.begin(), data.end()); // Append the rest of the data
        return rawData;
    }
};

// Base class for network manager
class NetworkManager
{
protected:
    std::queue<Packet> incomingPackets; // Queue for incoming packets
    std::queue<Packet> outgoingPackets; // Queue for outgoing packets
    std::atomic<bool> running;          // To control the network loop

    // Map to hold listeners for specific packet types
    std::unordered_map<uint8_t, std::function<void(const std::vector<uint8_t> &)>> listeners;

public:
    NetworkManager();
    virtual ~NetworkManager();

    // Enqueue outgoing packets
    void enqueueOutgoingPacket(const Packet &packet);

    // Register a listener for a specific packet type
    void registerListener(PacketType packetType, std::function<void(const std::vector<uint8_t> &)> callback);

    // Handle a received packet by triggering the corresponding listener
    void handlePacket(const Packet &packet);

    // To be implemented by derived classes to send outgoing packets
    virtual void sendOutgoingPackets() = 0;

    // Process incoming packets
    void processIncomingPackets();

    // Stop the network loop
    void stop();

    // Check if the network manager is running
    bool isRunning() const;
};

#endif // NETWORK_MANAGER_HPP
