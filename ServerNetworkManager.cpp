#include "ServerNetworkManager.hpp"
#include <iostream>
#include <stdexcept>

// Constructor
ServerNetworkManager::ServerNetworkManager(uint16_t port)
{
    ENetAddress address;
    enet_address_set_host(&address, "0.0.0.0");
    address.port = port;

    server = enet_host_create(&address, 32, 2, 0, 0); // 32 clients max, 2 channels
    if (!server)
    {
        throw std::runtime_error("Failed to create ENet server host.");
    }

    networkThread = std::thread(&ServerNetworkManager::networkLoop, this);
}

// Destructor
ServerNetworkManager::~ServerNetworkManager()
{
    enet_host_destroy(server);
    if (networkThread.joinable())
    {
        networkThread.join();
    }
}

// Main network loop to process ENet events
void ServerNetworkManager::networkLoop()
{
    Packet packet(PacketType::CUSTOM, {});
    while (running)
    {
        ENetEvent event;
        while (enet_host_service(server, &event, 1000) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "Client connected." << std::endl;
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                packet = parsePacket(event.packet);
                handlePacket(packet); // Handle the received packet
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Client disconnected." << std::endl;
                break;

            default:
                break;
            }
        }

        sendOutgoingPackets();
        processIncomingPackets();
    }
}

// Send all outgoing packets to connected clients
void ServerNetworkManager::sendOutgoingPackets()
{
    while (!outgoingPackets.empty())
    {
        Packet packet = outgoingPackets.front();
        outgoingPackets.pop();

        std::vector<uint8_t> rawData = packet.toRawData();
        for (size_t i = 0; i < server->peerCount; ++i)
        {
            ENetPacket *enetPacket = createENetPacket(packet);
            enet_peer_send(&server->peers[i], 0, enetPacket);
        }
    }
}
