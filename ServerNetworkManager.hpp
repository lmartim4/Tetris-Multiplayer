#ifndef SERVER_NETWORK_MANAGER_HPP
#define SERVER_NETWORK_MANAGER_HPP

#include "NetworkManager.hpp"
#include <enet/enet.h>
#include <thread>

class ServerNetworkManager : public NetworkManager {
private:
    ENetHost* server;
    std::thread networkThread;

public:
    ServerNetworkManager(uint16_t port = 12345);
    ~ServerNetworkManager();

    // Main network loop to handle events
    void networkLoop();

    // Send all outgoing packets to clients
    void sendOutgoingPackets() override;
};

#endif // SERVER_NETWORK_MANAGER_HPP
