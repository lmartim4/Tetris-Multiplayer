#ifndef CLIENT_NETWORK_MANAGER_HPP
#define CLIENT_NETWORK_MANAGER_HPP

#include "NetworkManager.hpp"
#include <chrono>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <thread>

class ClientManager : public NetworkManager {
private:
    ENetPeer* peer;
    std::thread actionThread;

public:
    ClientManager(const std::string& serverAddress = "localhost", uint16_t port = 12345);
    ~ClientManager();
    // Random action loop (called every 2 seconds)
    void actionLoop();
};

#endif // CLIENT_NETWORK_MANAGER_HPP
