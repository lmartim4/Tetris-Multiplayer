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
    ENetPeer *sourcePeer;      // The peer who sent the packet (if applicable)
    ENetPeer *destinationPeer; // The peer who will receive the packet (nullptr for broadcast)

    Packet(PacketType t, const std::vector<uint8_t> &d, ENetPeer *src = nullptr, ENetPeer *dest = nullptr)
        : type(t), data(d), sourcePeer(src), destinationPeer(dest) {}

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
    ENetHost *host;                     // The ENet host, used by both server and client
    std::atomic<bool> running;          // To control the network loop
    std::queue<Packet> outgoingPackets; // Queue for outgoing packets

    // Map to hold listeners for specific packet types
    std::unordered_map<uint8_t, std::function<void(const Packet &, ENetPeer *)>> listeners;

public:
    NetworkManager();
    virtual ~NetworkManager();

    // Enqueue outgoing packets
    void enqueueOutgoingPacket(const Packet &packet);

    // Register a listener for a specific packet type
    void registerListener(PacketType packetType, std::function<void(const Packet &, ENetPeer *)> callback);

    // Handle a received packet by triggering the corresponding listener
    void handlePacket(const Packet &packet, ENetPeer *peer);

    // Process incoming packets
    void processIncomingPackets();

    // Stop the network loop
    void stop();

    // Check if the network manager is running
    bool isRunning() const;

    // Common method: Parse the incoming ENet packet into a custom Packet structure
    Packet parsePacket(const ENetPacket *enetPacket, ENetPeer *sourcePeer);

    // Common method: Create an ENet packet from the custom Packet structure
    ENetPacket *createENetPacket(const Packet &packet);

    // Generalized network loop to handle events
    void networkLoop();

    // Send packets to the specified peer or broadcast
    void sendOutgoingPackets();

    // Send a packet to a specific peer
    void sendPacketToPeer(ENetPeer *peer, const Packet &packet);

    // Broadcast a packet to all connected peers
    void broadcastPacket(const Packet &packet);

    // Pure virtual method to handle ENet events (implementation is in derived classes)
    virtual void processENetEvent(ENetEvent &event) = 0;
};

#endif // NETWORK_MANAGER_HPP
