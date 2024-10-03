#include "NetworkManager.hpp"

// Constructor
NetworkManager::NetworkManager() : host(nullptr), running(true)
{
}

// Destructor
NetworkManager::~NetworkManager()
{
    stop();
}

// Enqueue an outgoing packet
void NetworkManager::send_packet(const Packet &packet)
{
    outgoingPackets.push(packet);
}

// Register a listener for a specific packet type
void NetworkManager::registerListener(PacketType packetType, std::function<void(const Packet &)> callback)
{
    listeners[static_cast<uint8_t>(packetType)] = callback;
}

// Handle a received packet by triggering the corresponding listener
void NetworkManager::handlePacket(Packet &packet, ENetPeer *peer)
{
    uint8_t packetType = static_cast<uint8_t>(packet.type);
    if (listeners.find(packetType) != listeners.end())
    {
        packet.peer = peer;
        listeners[packetType](packet); // Pass both packet and the source peer
    }
    else
    {
        std::cout << "No listener for packet - " << PacketTypeToString(packet.type) << "\n";
    }
}

// Process incoming packets
void NetworkManager::processIncomingPackets()
{
    while (!incommingPackets.empty())
    {
        Packet packet = incommingPackets.front();
        incommingPackets.pop();
        handlePacket(packet, packet.peer);
    }
}

// Stop the network loop
void NetworkManager::stop()
{
    running = false;
}

// Check if the network manager is running
bool NetworkManager::isRunning() const
{
    return running;
}

// Common method: Parse an incoming ENet packet into a custom Packet structure
Packet NetworkManager::parsePacket(const ENetPacket *enetPacket, ENetPeer *sourcePeer)
{
    if (enetPacket->dataLength < 1)
        return Packet(PacketType::CUSTOM, {}, sourcePeer);
    PacketType type = static_cast<PacketType>(enetPacket->data[0]);                             // First byte is the packet type
    std::vector<uint8_t> data(enetPacket->data + 1, enetPacket->data + enetPacket->dataLength); // Remaining data
    return Packet(type, data, sourcePeer);
}

// Common method: Create an ENet packet from a custom Packet structure
ENetPacket *NetworkManager::createENetPacket(const Packet &packet)
{
    std::vector<uint8_t> rawData = packet.toRawData();
    return enet_packet_create(rawData.data(), rawData.size(), ENET_PACKET_FLAG_RELIABLE);
}

// Generalized network loop to handle ENet events
void NetworkManager::networkLoop()
{
    while (running)
    {
        ENetEvent event;
        while (enet_host_service(host, &event, 100) > 0)
            processENetEvent(event); // Call the virtual function for specific event handling

        sendOutgoingPackets();
        processIncomingPackets();
    }
}

// Send packets to the specified peer or broadcast
void NetworkManager::sendOutgoingPackets()
{
    while (!outgoingPackets.empty())
    {
        Packet packet = outgoingPackets.front();
        outgoingPackets.pop();
        
        std::cout << "Sending a packet to enet sender\n";

        if (packet.peer)
        {
            ENetPacket *enetPacket = createENetPacket(packet);
            enet_peer_send(packet.peer, 0, enetPacket);
        }
        else
        {
            std::vector<uint8_t> rawData = packet.toRawData();
            for (size_t i = 0; i < host->peerCount; ++i)
            {
                ENetPacket *enetPacket = enet_packet_create(rawData.data(), rawData.size(), ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(&host->peers[i], 0, enetPacket);
            }
        }
    }
}

// Process ENet events for the server
void NetworkManager::processENetEvent(ENetEvent &event)
{
    switch (event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
        std::cout << "ENET_EVENT_TYPE_CONNECT" << std::endl;
        break;

    case ENET_EVENT_TYPE_RECEIVE:
    {
        Packet packet = parsePacket(event.packet, event.peer); // Parse packet with source peer
        handlePacket(packet, event.peer);                      // Handle the received packet
        enet_packet_destroy(event.packet);
    }
    break;

    case ENET_EVENT_TYPE_DISCONNECT:
        std::cout << "ENET_EVENT_TYPE_DISCONNECT " << std::endl;
        break;

    default:
        break;
    }
}