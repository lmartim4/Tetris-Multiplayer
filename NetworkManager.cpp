#include "NetworkManager.hpp"

// Constructor
NetworkManager::NetworkManager() : host(nullptr), running(true) {}

// Destructor
NetworkManager::~NetworkManager()
{
    stop();
}

// Enqueue an outgoing packet
void NetworkManager::enqueueOutgoingPacket(const Packet &packet)
{
    outgoingPackets.push(packet);
}

// Register a listener for a specific packet type
void NetworkManager::registerListener(PacketType packetType, std::function<void(const Packet &, ENetPeer *)> callback)
{
    listeners[static_cast<uint8_t>(packetType)] = callback;
}

// Handle a received packet by triggering the corresponding listener
void NetworkManager::handlePacket(const Packet &packet, ENetPeer *peer)
{
    uint8_t packetType = static_cast<uint8_t>(packet.type);
    if (listeners.find(packetType) != listeners.end())
    {
        listeners[packetType](packet, peer); // Pass both packet and the source peer
    }
}

// Process incoming packets
void NetworkManager::processIncomingPackets()
{
    while (!outgoingPackets.empty())
    {
        Packet packet = outgoingPackets.front();
        outgoingPackets.pop();
        handlePacket(packet, packet.sourcePeer);
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
        while (enet_host_service(host, &event, 1000) > 0)
        {
            processENetEvent(event); // Call the virtual function for specific event handling
        }

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

        if (packet.destinationPeer)
        {
            sendPacketToPeer(packet.destinationPeer, packet);
        }
        else
        {
            broadcastPacket(packet); // If no specific peer, broadcast to all
        }
    }
}

// Send a packet to a specific peer
void NetworkManager::sendPacketToPeer(ENetPeer *peer, const Packet &packet)
{
    ENetPacket *enetPacket = createENetPacket(packet);
    enet_peer_send(peer, 0, enetPacket);
}

// Broadcast a packet to all connected peers
void NetworkManager::broadcastPacket(const Packet &packet)
{
    std::vector<uint8_t> rawData = packet.toRawData();
    for (size_t i = 0; i < host->peerCount; ++i)
    {
        ENetPacket *enetPacket = enet_packet_create(rawData.data(), rawData.size(), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(&host->peers[i], 0, enetPacket);
    }
}
