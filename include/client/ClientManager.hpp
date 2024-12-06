#pragma once

#include "NetworkManager.hpp"
#include <SFML/Graphics.hpp>
#include "PlayerData.hpp"
#include <thread>
#include <atomic>

class ClientManager : public NetworkManager
{
private:
    ENetPeer *serverPeer;

    std::atomic<bool> isConnected = false;

    std::vector<PlayerData> players;

    std::thread ThreadHeartbeat;
    unsigned long last_heartbeat;

    bool debugEnabled = false;
    void handleKeyPress(sf::Event keyEvent, sf::RenderWindow &window);

protected:
    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;

public:
    ClientManager() {};
    ~ClientManager() {};

    void toggleDebug();
    void on_receive_heartbeat();

    void on_receive_player_list(const Packet &packet);

    void TaskStartHeartbeat();
    void TaskHeartbeat();
    void TaskStopHeartbeat();

    void connect(const std::string &serverAddress = "localhost", uint16_t port = 12345);
    void disconnect();

    bool IsConnected() const { return isConnected; };

    std::vector<PlayerData> &getDataPlayers()
    {
        return players;
    }
};