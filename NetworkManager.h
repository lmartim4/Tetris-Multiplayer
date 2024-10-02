// NetworkManager.hpp

#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <enet/enet.h>
#include <queue>
#include <vector>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <atomic>
#include <cstring>

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
    NetworkManager() : running(true) {}
    virtual ~NetworkManager() {
        stop();
    }

    void enqueueIncomingPacket(const Packet& packet) {
        incomingPackets.push(packet);
    }

    void enqueueOutgoingPacket(const Packet& packet) {
        outgoingPackets.push(packet);
    }

    virtual void sendOutgoingPackets() = 0;

    void processIncomingPackets() {
        while (!incomingPackets.empty()) {
            Packet packet = incomingPackets.front();
            incomingPackets.pop();
            // Process the packet here or trigger event handling
            std::cout << "Processing incoming packet" << std::endl;
        }
    }

    void stop() {
        running = false;
    }

    bool isRunning() const {
        return running;
    }
};

#endif // NETWORK_MANAGER_HPP
