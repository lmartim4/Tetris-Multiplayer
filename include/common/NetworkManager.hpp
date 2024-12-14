#pragma once

#include "PacketType.hpp"
#include <json.hpp>

#include <enet/enet.h>
#include <queue>
#include <vector>
#include <atomic>
#include <unordered_map>
#include <functional>
#include <thread>
#include <memory>

// This struct represents a network packet. It contains information such as the packet type,
// data payload, and the peer to which the packet is being sent or received from.
struct Packet
{
    PacketType type;           // The type of the packet (ACTION, HEARTBEAT, etc.)
    std::vector<uint8_t> data; // Data payload of the packet
    ENetPeer *peer;            // The peer that the packet is being sent to or received from

    // Constructor to initialize a packet with its type, data, and optional destination peer.
    Packet(PacketType t, const std::vector<uint8_t> &d, ENetPeer *dest = nullptr)
        : type(t), data(d), peer(dest) {}

    // New constructor that accepts a JSON object
    Packet(PacketType t, const nlohmann::json &j, ENetPeer *dest = nullptr)
        : type(t), peer(dest)
    {
        // Convert JSON to string
        std::string jsonStr = j.dump();
        // Store as bytes
        data.assign(jsonStr.begin(), jsonStr.end());
    }

    // Serialize the packet to raw byte data so it can be sent over the network.
    std::vector<uint8_t> toRawData() const
    {
        std::vector<uint8_t> rawData;
        rawData.push_back(static_cast<uint8_t>(type));           // First byte stores the packet type
        rawData.insert(rawData.end(), data.begin(), data.end()); // Append the rest of the data
        return rawData;
    }

    // Helper method to parse the data vector into a JSON object
    nlohmann::json toJson() const
    {
        std::string jsonStr(data.begin(), data.end());
        return nlohmann::json::parse(jsonStr);
    }
};

// The NetworkManager class is responsible for managing all network communication.
// It sets up the network host, handles incoming and outgoing packets, and provides
// methods to register listeners for specific packet types.
class NetworkManager
{
public:
    NetworkManager();          // Constructor: Initializes the ENet library and network manager.
    virtual ~NetworkManager(); // Destructor: Cleans up network resources and stops the network loop.

    static const int version = 1; // NetworkManager version for debugging purposes

    // Registers a listener function for a specific packet type.
    // The listener will be called whenever a packet of the given type is received.
    void registerListener(PacketType packetType, std::function<void(const Packet &)> callback);

    // Adds a packet to the outgoing packet queue to be sent to its destination peer.
    void send_packet(const Packet &packet);

    // Checks whether the network task (event loop) is currently running.
    bool isRunning() const;

    // Utility function to convert a 32-bit integer IP address to an IPv4 string (e.g., "192.168.1.1").
    static char *uint32_to_ipv4(uint32_t ip_addr);

    // Prints a message with a timestamp to the console for debugging purposes.
    static void network_print(const char *array);

    // Returns the ENetHost object, which represents either the client or server in ENet.
    ENetHost *getHost();

    std::vector<ENetPeer *> getPeers();

protected:
    ENetHost *host = nullptr; // ENet host, which can either be a server or a client.

    // Converts an ENet packet into a custom Packet structure by extracting the packet type and data.
    static Packet parsePacket(const ENetPacket *enetPacket, ENetPeer *sourcePeer);

    // Handles a received packet by calling the registered listener for its packet type.
    void handlePacket(Packet &packet, ENetPeer *peer);

    // Abstract method to handle when a peer connects to the host. Implemented by subclasses.
    virtual void onPeerConnect(ENetPeer *peer) = 0;

    // Abstract method to handle when a peer disconnects from the host. Implemented by subclasses.
    virtual void onPeerDisconnect(ENetPeer *peer) = 0;

    // Starts the network event loop in a separate thread to process incoming and outgoing packets.
    void TaskStartNetwork();

    // Stops the network loop and cleans up resources related to networking.
    void TaskStopNetwork();

private:
    std::thread networkThread;         // Thread for running the network event loop.
    std::atomic<bool> running = false; // Atomic flag to indicate whether the network loop is active.

    // Queue for outgoing packets that need to be sent to peers.
    std::queue<Packet> outgoingPackets;

    // Queue for incoming packets that have been received but not yet processed.
    std::queue<Packet> incommingPackets;

    // A map that associates packet types (as uint8_t) with their corresponding listener functions.
    std::unordered_map<uint8_t, std::function<void(const Packet &)>> listeners;

    // Processes events received from ENet, such as peer connections, disconnections, and packet receptions.
    void processENetEvent(ENetEvent &event);

    // Processes any incoming packets in the queue and triggers the appropriate listeners.
    void processIncomingPackets();

    // Sends any outgoing packets in the queue to their destination peers or broadcasts them to all peers.
    void sendOutgoingPackets();

    // Creates an ENet packet from a custom Packet structure, ready to be sent over the network.
    ENetPacket *createENetPacket(const Packet &packet);

    // The main network loop that continuously processes ENet events, sends outgoing packets, and handles incoming ones.
    void TaskNetwork();
};
