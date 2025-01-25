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

int ServerManager::getNextAvailableid()
{
    static int currentID = 0;
    return currentID++;
}

void ServerManager::createPlayerAndLink(ENetPeer *peer)
{
    Player *newPlayer = new Player(getNextAvailableid(), "Player_");
    peer->data = (void *)newPlayer;
    std::cout << "New client connected. Assigned id: " << newPlayer->getData().id << "\n";
    players.addPlayer((*newPlayer).getData());
}

void ServerManager::broadcastPlayerList()
{
    std::cout << players.serialize() << std::endl;
    sendPacket(Packet(PacketType::PLAYER_LIST, players, nullptr));
}

void ServerManager::broadcastSound(SoundType soundType)
{
    Packet playSoundPacket(PacketType::PLAY_SOUND, (uint8_t)soundType, nullptr);
    sendPacket(playSoundPacket);
}

void ServerManager::broadcast_starting_game()
{
    sendPacket(Packet(PacketType::STARTING_GAME, nullptr));
}