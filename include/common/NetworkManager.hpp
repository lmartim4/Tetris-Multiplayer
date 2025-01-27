#pragma once

#include "Packet.hpp"
#include <json.hpp>

#include <enet/enet.h>
#include <queue>
#include <vector>
#include <atomic>
#include <unordered_map>
#include <functional>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "Logger.hpp"

// The NetworkManager class is responsible for managing all network communication.
// It sets up the network host, handles incoming and outgoing packets, and provides
// methods to register listeners for specific packet types.
class NetworkManager
{
public:
    const int network_frequency = 128; // Hertz
    NetworkManager();                  // Constructor: Initializes the ENet library and network manager.
    virtual ~NetworkManager();         // Destructor: Cleans up network resources and stops the network loop.

    static const int version = 1; // NetworkManager version for debugging purposes

    // Registers a listener function for a specific packet type.
    // The listener will be called whenever a packet of the given type is received.
    void registerListener(PacketType packetType, std::function<void(const Packet &)> callback);

    // Checks whether the network task (event loop) is currently running.
    bool isRunning() const;

    // Utility function to convert a 32-bit integer IP address to an IPv4 string (e.g., "192.168.1.1").
    std::string uint32_to_ipv4(uint32_t ip_addr);

    // Returns the ENetHost object, which represents either the client or server in ENet.
    ENetHost *getHost();

    std::vector<ENetPeer *> getPeers();

    // Send a packet to a specific destination
    virtual void sendPacket(const Packet &packet);

    // Broadcast a packet to all connected peers
    virtual void broadcastPacket(const Packet &packet);

protected:
    Logger *logger;
    ENetHost *host = nullptr; // ENet host, which can either be a server or a client.

    // Converts an ENet packet into a custom Packet structure by extracting the packet type and data.
    Packet parsePacket(const ENetPacket *enetPacket, ENetPeer *sourcePeer);

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
    std::thread networkThread; // Thread for running the network event loop.

    std::thread sendThread;
    std::thread receiveThread;

    std::atomic<bool> mainTask = false; // Atomic flag to indicate whether the network loop is active.
    std::atomic<bool> sending = false;
    std::atomic<bool> receiving = false;

    // Queue for outgoing packets that need to be sent to peers.
    std::queue<Packet> outgoingPackets;

    // Queue for incoming packets that have been received but not yet processed.
    std::queue<Packet> incommingPackets;

    // A map that associates packet types (as uint8_t) with their corresponding listener functions.
    std::unordered_map<uint8_t, std::function<void(const Packet &)>> listeners;

    std::mutex outgoingMutex;
    std::mutex incomingMutex;

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
    void TaskSend();
    void TaskReceive();
};
