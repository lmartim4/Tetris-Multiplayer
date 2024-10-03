#ifndef CLIENT_NETWORK_MANAGER_HPP
#define CLIENT_NETWORK_MANAGER_HPP

#include "NetworkManager.hpp"
#include <chrono>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <thread>

class ClientNetworkManager : public NetworkManager {
private:
    ENetPeer* peer;
    std::thread networkThread;
    std::thread actionThread;

public:
    ClientNetworkManager(const std::string& serverAddress = "localhost", uint16_t port = 12345);
    ~ClientNetworkManager();

    // Process ENet events specifically for client (handle server connections, etc.)
    void processENetEvent(ENetEvent& event) override;

    // Random action loop (called every 2 seconds)
    void actionLoop();
};

#endif // CLIENT_NETWORK_MANAGER_HPP
