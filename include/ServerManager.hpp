#pragma once

#include "NetworkManager.hpp"
#include <enet/enet.h>
#include <thread>

class ServerManager : public NetworkManager
{
private:
protected:
    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;

public:
    ServerManager(uint16_t port = 12345);
    ~ServerManager();
};
