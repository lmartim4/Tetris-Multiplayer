#include "ServerManager.hpp"
#include <iostream>
#include <stdexcept>

#define MAX_CLIENTS 32
#define CHANNELS 2

void ServerManager::onPeerConnect(ENetPeer *peer)
{
    network_print("");
    std::cout << "[CONNECTED] " << uint32_to_ipv4(peer->address.host) << ":" << peer->address.port << std::endl;

    createPlayerAndLink(peer);
    broadcastPlayerList();
}
void ServerManager::onPeerDisconnect(ENetPeer *peer)
{
    network_print("");
    std::cout << "[DISCONNECTED] " << uint32_to_ipv4(peer->address.host) << ":" << peer->address.port << std::endl;
    delete (Player *)peer->data;
    broadcastPlayerList();
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

int ServerManager::getNextAvailablePlayerID()
{
    static int currentID = 0;
    return currentID++;
}

void ServerManager::createPlayerAndLink(ENetPeer *peer)
{
    Player *newPlayer = new Player(getNextAvailablePlayerID(), "Unnamed");
    peer->data = (void *)newPlayer;
    std::cout << "New client connected. Assigned PlayerID: " << newPlayer->getData().playerID << "\n";
}

void ServerManager::broadcastPlayerList()
{
    nlohmann::json message = nlohmann::json::array();

    for (ENetPeer *entry : getPeers())
    {
        if (!entry->data)
        {
            std::cout << "~ No player ~" << std::endl;
            continue;
        }

        Player *player = (Player *)entry->data;

        nlohmann::json playerJson = player->getData().serialize();

        message.push_back(nlohmann::json::array({playerJson}));
    }

    std::cout << message << std::endl;
    sendPacket(Packet(PacketType::PLAYER_LIST, message, nullptr));
}

void ServerManager::broadcast_starting_game()
{
    sendPacket(Packet(PacketType::STARTING_GAME, 0, nullptr));
}