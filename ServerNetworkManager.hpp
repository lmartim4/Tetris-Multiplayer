#ifndef SERVER_NETWORK_MANAGER_HPP
#define SERVER_NETWORK_MANAGER_HPP

#include "NetworkManager.hpp"
#include <enet/enet.h>
#include <thread>

class ServerNetworkManager : public NetworkManager {
private:
    std::thread networkThread;

public:
    ServerNetworkManager(uint16_t port = 12345);
    ~ServerNetworkManager();

    // Process ENet events specifically for server (handle client connections, etc.)
    void processENetEvent(ENetEvent& event) override;
};

#endif // SERVER_NETWORK_MANAGER_HPP
