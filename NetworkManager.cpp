#include "NetworkManager.hpp"

NetworkManager::NetworkManager() : host(nullptr), running(true)
{
}
NetworkManager::~NetworkManager()
{
    stop();
}

void NetworkManager::send_packet(const Packet &packet)
{
    outgoingPackets.push(packet);
}

void NetworkManager::registerListener(PacketType packetType, std::function<void(const Packet &)> callback)
{
    listeners[static_cast<uint8_t>(packetType)] = callback;
}

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
        network_print("");
        std::cout << "[" << peer->address.host << ":" << peer->address.port << "]" << " >> \"" << PacketTypeToString(packet.type) << "\" (No listener registred)\n";
    }
}

void NetworkManager::processIncomingPackets()
{
    while (!incommingPackets.empty())
    {
        Packet packet = incommingPackets.front();
        incommingPackets.pop();
        handlePacket(packet, packet.peer);
    }
}

void NetworkManager::stop()
{
    running = false;
}

bool NetworkManager::isRunning() const
{
    return running;
}

Packet NetworkManager::parsePacket(const ENetPacket *enetPacket, ENetPeer *sourcePeer)
{
    if (enetPacket->dataLength < 1)
        return Packet(PacketType::CUSTOM, {}, sourcePeer);
    PacketType type = static_cast<PacketType>(enetPacket->data[0]);                             // First byte is the packet type
    std::vector<uint8_t> data(enetPacket->data + 1, enetPacket->data + enetPacket->dataLength); // Remaining data
    return Packet(type, data, sourcePeer);
}

ENetPacket *NetworkManager::createENetPacket(const Packet &packet)
{
    std::vector<uint8_t> rawData = packet.toRawData();
    return enet_packet_create(rawData.data(), rawData.size(), ENET_PACKET_FLAG_RELIABLE);
}

void NetworkManager::networkLoop()
{
    int network_frequencie = 20; // Hertz
    while (running)
    {
        ENetEvent event;

        while (enet_host_service(host, &event, (1000 / network_frequencie)) > 0)
            processENetEvent(event);

        sendOutgoingPackets();
        processIncomingPackets();
    }
}

void NetworkManager::sendOutgoingPackets()
{
    while (!outgoingPackets.empty())
    {
        Packet packet = outgoingPackets.front();
        outgoingPackets.pop();

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

void NetworkManager::processENetEvent(ENetEvent &event)
{
    switch (event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
        network_print("");
        std::cout << "[CONNECT] " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
        break;

    case ENET_EVENT_TYPE_RECEIVE:
    {
        Packet packet = parsePacket(event.packet, event.peer);
        handlePacket(packet, event.peer);
        enet_packet_destroy(event.packet);
    }
    break;

    case ENET_EVENT_TYPE_DISCONNECT:
        network_print("");
        std::cout << "[DISCONNECT] " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
        break;

    default:
        std::cout << "[ENET] EventType = " << event.type << std::endl;
        break;
    }
}