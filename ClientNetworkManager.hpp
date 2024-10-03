#ifndef CLIENT_NETWORK_MANAGER_HPP
#define CLIENT_NETWORK_MANAGER_HPP

#include "NetworkManager.hpp"
#include <chrono>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <thread>

class ClientNetworkManager : public NetworkManager {
private:
    ENetHost* client;
    ENetPeer* peer;
    std::thread networkThread;
    std::thread actionThread;

public:
    ClientNetworkManager(const std::string& serverAddress = "localhost", uint16_t port = 12345);
    ~ClientNetworkManager();

    // Main network loop to handle events and packets
    void networkLoop();

    // Send a specific command using PacketType
    void sendCommand(PacketType commandType);

    // Send outgoing packets to the server
    void sendOutgoingPackets() override;

    // Random action loop (called every 2 seconds)
    void actionLoop();
};

#endif // CLIENT_NETWORK_MANAGER_HPP
