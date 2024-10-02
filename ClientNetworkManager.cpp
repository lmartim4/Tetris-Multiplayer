#include "ClientNetworkManager.hpp"
#include "PacketType.hpp"
#include <iostream>

// Constructor
ClientNetworkManager::ClientNetworkManager(const std::string &serverAddress, uint16_t port)
{
    client = enet_host_create(nullptr, 1, 2, 0, 0); // 1 client, 2 channels
    if (!client)
    {
        throw std::runtime_error("Failed to create ENet client host.");
    }

    ENetAddress address;
    enet_address_set_host(&address, serverAddress.c_str());
    address.port = port;

    peer = enet_host_connect(client, &address, 2, 0);
    if (!peer)
    {
        throw std::runtime_error("No available peers for initiating an ENet connection.");
    }

    networkThread = std::thread(&ClientNetworkManager::networkLoop, this);
    actionThread = std::thread(&ClientNetworkManager::actionLoop, this);

    // Seed random number generator
    std::srand(std::time(0));
}

// Destructor
ClientNetworkManager::~ClientNetworkManager()
{
    enet_host_destroy(client);
    if (networkThread.joinable())
    {
        networkThread.join();
    }
    if (actionThread.joinable())
    {
        actionThread.join();
    }
}

// Main network loop
void ClientNetworkManager::networkLoop()
{
    while (running)
    {
        ENetEvent event;
        while (enet_host_service(client, &event, 1000) > 0)
        {
            Packet packet(PacketType::CUSTOM, {}); // Declare the packet variable outside the switch block

            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "Connected to server." << std::endl;
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                packet = parsePacket(event.packet); // Now you can assign to packet here
                handlePacket(packet);               // Call handlePacket to trigger listeners
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnected from server." << std::endl;
                peer = nullptr;
                break;

            default:
                break;
            }
        }

        sendOutgoingPackets();
        processIncomingPackets();
    }
}

// Function to send a specific command
void ClientNetworkManager::sendCommand(PacketType commandType)
{
    std::vector<uint8_t> data = {};          // Empty data, you can modify this if needed
    Packet commandPacket(commandType, data); // Explicit PacketType

    // Send the packet
    enqueueOutgoingPacket(commandPacket);

    // Print the sent command
    std::cout << "Sent command: " << PacketTypeToString(commandType) << std::endl;
}

// Function that runs every 2 seconds to send random command
void ClientNetworkManager::actionLoop()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Send a random command
        int randomIndex = std::rand() % 17;

        sendCommand(PacketType::JOIN_REQUEST);
        // sendCommand((PacketType)randomIndex);
    }
}

// Send all outgoing packets
void ClientNetworkManager::sendOutgoingPackets()
{
    while (!outgoingPackets.empty())
    {
        Packet packet = outgoingPackets.front();
        outgoingPackets.pop();

        // Convert packet to raw data and send it
        std::vector<uint8_t> rawData = packet.toRawData();
        ENetPacket *enetPacket = enet_packet_create(rawData.data(), rawData.size(), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, enetPacket);
    }
}

// Parse an incoming ENet packet into a custom Packet structure
Packet ClientNetworkManager::parsePacket(ENetPacket *enetPacket)
{
    if (enetPacket->dataLength < 1)
        return Packet(PacketType::CUSTOM, {});

    PacketType type = static_cast<PacketType>(enetPacket->data[0]);                             // First byte is the packet type
    std::vector<uint8_t> data(enetPacket->data + 1, enetPacket->data + enetPacket->dataLength); // Remaining data
    return Packet(type, data);
}

// Create an ENet packet from a custom Packet structure
ENetPacket *ClientNetworkManager::createENetPacket(const Packet &packet)
{
    return enet_packet_create(packet.data.data(), packet.data.size(), ENET_PACKET_FLAG_RELIABLE);
}
