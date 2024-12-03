#pragma once

#include "NetworkManager.hpp"
#include <SFML/Graphics.hpp>
#include <thread>
#include <atomic>

class ClientManager : public NetworkManager
{
private:
    ENetPeer *serverPeer;

    std::thread ThreadHeartbeat;
    std::thread ThreadSFML;

    std::atomic<bool> sfml_running;

    bool isConnected = false;
    unsigned long last_heartbeat;

    bool debugEnabled = false;
    void handleKeyPress(sf::Event keyEvent, sf::RenderWindow &window);

protected:
    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;

public:
    ClientManager(const std::string &serverAddress = "localhost", uint16_t port = 12345);
    ~ClientManager();

    void toggleDebug();
    void on_receive_heartbeat();

    void TaskStartHeartbeat();
    void TaskHeartbeat();
    void TaskStopHeartbeat();

    void TaskStartSFML();
    void TaskSFML();
    void TaskStopSFML();
};