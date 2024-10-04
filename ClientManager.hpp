#ifndef CLIENT_NETWORK_MANAGER_HPP
#define CLIENT_NETWORK_MANAGER_HPP

#include "NetworkManager.hpp"
#include <SFML/Graphics.hpp>
#include <thread>
#include <atomic>

class ClientManager : public NetworkManager
{
private:
    ENetPeer *serverPeer;
    std::thread actionThread;
    std::thread windowThread;
    bool isConnected = false;
    unsigned long last_heartbeat;
    std::atomic<bool> sfml_running;
    bool debugEnabled = false;

public:
    ClientManager(const std::string &serverAddress = "localhost", uint16_t port = 12345);
    ~ClientManager();

    void HEARTBEAT_TASK();
    void heartbeat_listener();

    void initializeSFML();

    void windowLoop();
    void stopWindow();

    void toggleDebug();
};

#endif // CLIENT_NETWORK_MANAGER_HPP
