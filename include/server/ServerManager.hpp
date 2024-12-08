#pragma once

#include <enet/enet.h>
#include <thread>
#include <map>

#include "NetworkManager.hpp"
#include "game/GameManager.hpp"
#include "Player.hpp"
#include <json.hpp>
#include <random>

class ServerManager : public NetworkManager
{
private:
    int getNextAvailablePlayerID();
    void broadcastPlayerList();
    void createPlayerAndLink(ENetPeer *peer);

protected:
    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;

public:
    ServerManager(uint16_t port = 12345);
    ~ServerManager() {};

    void start_game();
};
