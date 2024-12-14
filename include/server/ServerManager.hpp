#pragma once

#include <enet/enet.h>
#include <thread>
#include <map>

#include "NetworkManager.hpp"
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

    void broadcast_starting_game();

    static Player *extractPlayerFromPacket(const Packet &packet)
    {
        if (!packet.peer)
        {
            throw std::runtime_error("Error: Packet has no associated ENetPeer.");
        }
        if (!packet.peer->data)
        {
            throw std::runtime_error("Error: ENetPeer has no associated Player.");
        }

        return static_cast<Player *>(packet.peer->data);
    }
};
