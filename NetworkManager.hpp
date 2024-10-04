#pragma once

#include "PacketType.hpp"
#include <enet/enet.h>
#include <queue>
#include <vector>
#include <atomic>
#include <unordered_map>
#include <functional>
#include <thread>

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
public:
    NetworkManager();
    virtual ~NetworkManager();

    static const int version = 1;

    void registerListener(PacketType packetType, std::function<void(const Packet &)> callback); // Register a listener for a specific packet type
    void send_packet(const Packet &packet);                                                     // Enqueue outgoing packets
    bool isRunning() const;                                                                     // Check if the network task is running

    static char *uint32_to_ipv4(uint32_t ip_addr); // Function to convert uint32_t to IPv4 string
    static void network_print(const char *array);

    ENetHost *getHost();

protected:
    ENetHost *host = nullptr; // The ENet host, used by both server and client
    static Packet parsePacket(const ENetPacket *enetPacket, ENetPeer *sourcePeer);

    void handlePacket(Packet &packet, ENetPeer *peer); // Handle a received packet by triggering the corresponding listener

    virtual void onPeerConnect(ENetPeer *peer) = 0;
    virtual void onPeerDisconnect(ENetPeer *peer) = 0;

    void TaskStartNetwork(); // Start Event Network Loop
private:
    std::thread networkThread;
    std::atomic<bool> running = false;                                          // To control the network loop
    std::queue<Packet> outgoingPackets;                                         // Queue for outgoing packets
    std::queue<Packet> incommingPackets;                                        // Queue for outgoing packets
    std::unordered_map<uint8_t, std::function<void(const Packet &)>> listeners; // Map to hold listeners for specific packet types

    void processENetEvent(ENetEvent &event); // handle ENet events (implementation is in derived classes)
    void processIncomingPackets();
    void sendOutgoingPackets(); // Deliver packets to the specified peer or broadcast

    ENetPacket *createENetPacket(const Packet &packet); // Common method: Create an ENet packet from the custom Packet structure

    void TaskNetwork(); // network loop to handle events
    void TaskStopNetwork(); // Stop the network loop
};