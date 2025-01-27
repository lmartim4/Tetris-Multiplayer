#include "ServerManager.hpp"
#include <iostream>
#include <stdexcept>

#define MAX_CLIENTS 32
#define CHANNELS 2

void ServerManager::onPeerConnect(ENetPeer *peer)
{
    logger->console_log("[CONNECTED] " + uint32_to_ipv4(peer->address.host) + ":" + std::to_string(peer->address.port));

    createPlayerAndLink(peer);
    broadcastPlayerList();

    broadcastSound(SoundType::OnJoinLobby);
}

void ServerManager::onPeerDisconnect(ENetPeer *peer)
{
    logger->console_log("[DISCONNECTED] " + uint32_to_ipv4(peer->address.host) + ":" + std::to_string(peer->address.port));

    players.removePlayer(((Player *)peer->data)->getData());
    broadcastPlayerList();

    delete (Player *)peer->data;
}

ServerManager::ServerManager(uint16_t port)
{
    std::cout << std::endl;
    std::cout << "╭───────────────────────────────────────╮" << std::endl;
    std::cout << "│          Starting Tetris Server       │" << std::endl;
    std::cout << "├───────────────────────────────────────┤" << std::endl;
    std::cout << "│   Using NetworkManager version v" << NetworkManager::version << "     │" << std::endl;
    std::cout << "│   Opening server at port " << port << "        │" << std::endl;
    std::cout << "╰───────────────────────────────────────╯" << std::endl;
    std::cout << std::endl;

    ENetAddress address;
    enet_address_set_host(&address, "0.0.0.0");
    address.port = port;

    try
    {
        host = enet_host_create(&address, MAX_CLIENTS, CHANNELS, 0, 0);
        if (!host)
            throw std::runtime_error("Failed to create ENet server host. Is it already running? (Port busy)");
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return;
    }

    TaskStartNetwork();
}

ServerManager::~ServerManager()
{
    std::cout << "Ending ServerManager\n";
}

int ServerManager::getNextAvailableid()
{
    static int currentID = 0;
    return currentID++;
}

void ServerManager::createPlayerAndLink(ENetPeer *peer)
{
    Player *newPlayer = new Player(getNextAvailableid(), "Player_");
    peer->data = (void *)newPlayer;

    logger->console_log("New client connected. Assigned id: " + std::to_string(newPlayer->getData().id));
    players.addPlayer((*newPlayer).getData());
}

void ServerManager::broadcastPlayerList()
{
    // std::cout << players.serialize() << std::endl;
    sendPacket(Packet(PacketType::PLAYER_LIST, players, nullptr));
}

void ServerManager::broadcastSound(SoundType soundType)
{
    sendPacket(Packet(PacketType::PLAY_SOUND, (uint8_t)soundType, nullptr));
}

void ServerManager::playSoundToPlayer(const Player *player, SoundType soundType)
{
    sendPacket(Packet(PacketType::PLAY_SOUND, (uint8_t)soundType, getEnetPeerByPlayer(player)));
}

void ServerManager::broadcastBackgroundSound(SoundType sound)
{
    sendPacket(Packet(PacketType::SET_BACKGROUND_SOUND, (uint8_t)sound, nullptr));
}

void ServerManager::broadcast_starting_game()
{
    sendPacket(Packet(PacketType::STARTING_GAME, nullptr));
}

ENetPeer *ServerManager::getEnetPeerByPlayer(const Player *player)
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
                return peer;
        }
    }

    return nullptr;
}

Player *ServerManager::getPlayerFromPacket(const Packet &packet)
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

std::vector<Player *> ServerManager::getPlayers()
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