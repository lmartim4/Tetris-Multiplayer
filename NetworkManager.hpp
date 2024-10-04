#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include "PacketType.hpp"
#include <enet/enet.h>
#include <iomanip>
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
    ENetPeer *peer;

    Packet(PacketType t, const std::vector<uint8_t> &d, ENetPeer *dest = nullptr)
        : type(t), data(d), peer(dest) {}

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
private:
    std::atomic<bool> running = false;           // To control the network loop
    std::queue<Packet> outgoingPackets;  // Queue for outgoing packets
    std::queue<Packet> incommingPackets; // Queue for outgoing packets
    // Map to hold listeners for specific packet types
    std::unordered_map<uint8_t, std::function<void(const Packet &)>> listeners;

    // Process incoming packets
    void processIncomingPackets();

    // Stop the network loop
    void stopNetwork();

    // Common method: Create an ENet packet from the custom Packet structure
    ENetPacket *createENetPacket(const Packet &packet);

    // Generalized network loop to handle events
    void networkLoop();

    // Send packets to the specified peer or broadcast
    void sendOutgoingPackets();

    // handle ENet events (implementation is in derived classes)
    void processENetEvent(ENetEvent &event);

protected:
    std::thread networkThread;
    ENetHost *host = nullptr; // The ENet host, used by both server and client

    static Packet parsePacket(const ENetPacket *enetPacket, ENetPeer *sourcePeer);

    // Handle a received packet by triggering the corresponding listener
    void handlePacket(Packet &packet, ENetPeer *peer);

public:
    NetworkManager();
    virtual ~NetworkManager();

    static const int version = 1;

    // Register a listener for a specific packet type
    void registerListener(PacketType packetType, std::function<void(const Packet &)> callback);

    // Enqueue outgoing packets
    void send_packet(const Packet &packet);

    // Check if the network manager is running
    bool isRunning() const;

    void startNetworkTask();

    void network_print(const char *array);

    ENetHost *getHost()
    {
        return host;
    }

    // Function to convert uint32_t to IPv4 string
    char *uint32_to_ipv4(uint32_t ip_addr);
};

#endif // NETWORK_MANAGER_HPP
