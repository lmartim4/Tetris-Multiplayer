#pragma once

#include <enet/enet.h>
#include <thread>
#include <map>

#include "SoundSender.hpp"
#include "NetworkManager.hpp"

#include "Player.hpp"
#include "SoundType.hpp"
#include "PlayerList.hpp"

#include <json.hpp>
#include <random>

class ServerManager : public NetworkManager, public SoundSender
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
    ~ServerManager();

    void stop()
    {
        TaskStopNetwork();
    }
    
    void broadcastSound(SoundType soundType) override;
    void playSoundToPlayer(const Player *player, SoundType soundType) override;
    void broadcastBackgroundSound(SoundType sound) override;

    void broadcast_starting_game();

    ENetPeer *getEnetPeerByPlayer(const Player *player);

    static Player *getPlayerFromPacket(const Packet &packet);

    std::vector<Player *> getPlayers();
};
