#ifndef CLIENT_NETWORK_MANAGER_HPP
#define CLIENT_NETWORK_MANAGER_HPP

#include "NetworkManager.hpp"
#include <chrono>
#include <cstdlib> // For rand() and srand()
#include <chrono>
#include <ctime> // For time()
#include <thread>

class ClientManager : public NetworkManager
{
private:
    ENetPeer *peer;
    std::thread actionThread;
    bool isConnected = false;
    unsigned long last_heartbeat;

public:
    ClientManager(const std::string &serverAddress = "localhost", uint16_t port = 12345);
    ~ClientManager();

    void on_receive_heartbeat()
    {
        // Get the current time point
        auto now = std::chrono::system_clock::now();

        // Convert the time point to milliseconds since epoch
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        last_heartbeat = now_ms;
    }
    void HEARTBEAT_TASK();
};

#endif // CLIENT_NETWORK_MANAGER_HPP
