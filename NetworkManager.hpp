// NetworkManager.hpp

#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <enet/enet.h>
#include <queue>
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>

// Structure to represent a packet
struct Packet {
    std::vector<uint8_t> data;

    Packet(const std::vector<uint8_t>& d) : data(d) {}
};

// Base class for network manager
class NetworkManager {
protected:
    std::queue<Packet> incomingPackets;     // Queue for incoming packets
    std::queue<Packet> outgoingPackets;     // Queue for outgoing packets
    std::atomic<bool> running;              // To control the network loop

public:
    NetworkManager();
    virtual ~NetworkManager();

    // Enqueue incoming packets
    void enqueueIncomingPacket(const Packet& packet);

    // Enqueue outgoing packets
    void enqueueOutgoingPacket(const Packet& packet);

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
