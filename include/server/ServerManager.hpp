#pragma once

#include <enet/enet.h>
#include <thread>
#include <map>

#include "NetworkManager.hpp"

#include "Player.hpp"
#include "SoundType.hpp"
#include "PlayerList.hpp"

#include <json.hpp>
#include <random>

class ServerManager : public NetworkManager
{
private:
    int getNextAvailableid();
    void broadcastPlayerList();
    void createPlayerAndLink(ENetPeer *peer);
    PlayerList players;

protected:
    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;

public:
    ServerManager(uint16_t port = 12345);
    ~ServerManager() {};

    void broadcastSound(SoundType soundType);

    void broadcast_starting_game();

    ENetPeer *getEnetPeerByPlayer(Player *player)
    {
        if (!player)
        {
            throw std::invalid_argument("Player pointer cannot be null.");
        }

        for (ENetPeer *peer : getPeers())
        {
            if (peer->data)
            {
                Player *linkedPlayer = static_cast<Player *>(peer->data);
                if (linkedPlayer == player)
                {
                    return peer;
                }
            }
        }

        // If no matching ENetPeer is found
        return nullptr;
    }

    static Player *getPlayerFromPacket(const Packet &packet)
    {
        if (!packet.getPeer())
        {
            throw std::runtime_error("Error: Packet has no associated ENetPeer.");
        }
        if (!packet.getPeer()->data)
        {
            throw std::runtime_error("Error: ENetPeer has no associated Player.");
        }

        return static_cast<Player *>(packet.getPeer()->data);
    }

    std::vector<Player *> getPlayers()
    {
        std::vector<Player *> players;

        for (ENetPeer *entry : getPeers())
        {
            if (!entry->data)
            {
                std::cout << "~No player~" << std::endl;
                continue;
            }
            Player *player = (Player *)entry->data;

            players.emplace_back(player);
        }
        return players;
    }
};
