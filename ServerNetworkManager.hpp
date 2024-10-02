#ifndef SERVER_NETWORK_MANAGER_HPP
#define SERVER_NETWORK_MANAGER_HPP

#include "NetworkManager.hpp"
#include <enet/enet.h>
#include <thread>

class ServerNetworkManager : public NetworkManager
{ // Public inheritance
private:
    ENetHost *server;
    std::thread networkThread;

public:
    ServerNetworkManager(uint16_t port = 12345);
    ~ServerNetworkManager();

    // Main network loop to handle events
    void networkLoop();

    // Send all outgoing packets to clients
    void sendOutgoingPackets() override;

    // Parse incoming ENet packet into the custom Packet structure
    Packet parsePacket(ENetPacket *enetPacket);

    // Create an ENet packet from the internal Packet structure
    ENetPacket *createENetPacket(const Packet &packet);
};

#endif // SERVER_NETWORK_MANAGER_HPP
