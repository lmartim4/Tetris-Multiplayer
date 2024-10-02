// ClientNetworkManager.hpp

#ifndef CLIENT_NETWORK_MANAGER_HPP
#define CLIENT_NETWORK_MANAGER_HPP

#include "NetworkManager.hpp"
#include <chrono>
#include <thread>

class ClientNetworkManager : public NetworkManager {
private:
    ENetHost* client;
    ENetPeer* peer;
    std::thread networkThread;
    std::thread heartbeatThread;

public:
    ClientNetworkManager(const std::string& serverAddress = "localhost", uint16_t port = 12345);
    ~ClientNetworkManager();

    // Main network loop to handle events and packets
    void networkLoop();

    // Send heartbeat packet every second
    void sendHeartbeatLoop();

    // Send all outgoing packets to the server
    void sendOutgoingPackets() override;

    // Parse the incoming ENet packet
    Packet parsePacket(ENetPacket* enetPacket);

    // Create an ENet packet from the internal Packet structure
    ENetPacket* createENetPacket(const Packet& packet);
};

#endif // CLIENT_NETWORK_MANAGER_HPP
