#include "NetworkManager.hpp"
#include <iomanip>
#include <thread>

std::string NetworkManager::uint32_to_ipv4(uint32_t ip_addr)
{
    ip_addr = htonl(ip_addr);
    char ip_str[16]; // Temporary buffer for formatting
    snprintf(ip_str, sizeof(ip_str), "%u.%u.%u.%u",
             (ip_addr >> 24) & 0xFF,
             (ip_addr >> 16) & 0xFF,
             (ip_addr >> 8) & 0xFF,
             ip_addr & 0xFF);
    return std::string(ip_str); // Return as a std::string
}

NetworkManager::NetworkManager()
{
    logger = new Logger("NetworkManager");
    if (enet_initialize() != 0)
    {
        std::cerr << "Failed to initialize ENET!" << std::endl;
        throw std::runtime_error("Failed to initialize ENet.");
    }
}

NetworkManager::~NetworkManager()
{
    TaskStopNetwork();
}

void NetworkManager::sendPacket(const Packet &packet)
{
    outgoingPackets.push(packet);
}
void NetworkManager::broadcastPacket(const Packet &packet)
{

    outgoingPackets.push(packet);
}

void NetworkManager::TaskStartNetwork()
{
    if (mainTask)
    {
        std::cout << "Error: Network Task already running! (Why would you start again?)" << std::endl;
        return;
    }

    logger->console_log("Initializing Network Task...\n");

    mainTask = true;
    sending = true;
    receiving = true;

    networkThread = std::thread(&NetworkManager::TaskNetwork, this);
    sendThread = std::thread(&NetworkManager::TaskSend, this);
    receiveThread = std::thread(&NetworkManager::TaskReceive, this);
}

void NetworkManager::registerListener(PacketType packetType, std::function<void(const Packet &)> callback)
{
    listeners[static_cast<uint8_t>(packetType)] = callback;
}

void NetworkManager::handlePacket(Packet &packet, ENetPeer *peer)
{
    uint8_t packetType = static_cast<uint8_t>(packet.getType());

    if (listeners.find(packetType) != listeners.end())
    {
        packet.setENetPeer(peer);
        listeners[packetType](packet); // Pass both packet and the source peer
    }
    else
    {
        logger->console_log("[" + uint32_to_ipv4(peer->address.host) + ":" + std::to_string(peer->address.port) + "] >> \"" +
                            PacketTypeToString(packet.getType()) + "\" (No listener registered)\n");
    }
}

void NetworkManager::processIncomingPackets()
{
    while (!incommingPackets.empty())
    {
        Packet packet = incommingPackets.front();
        incommingPackets.pop();
        handlePacket(packet, packet.getPeer());
    }
}

void NetworkManager::TaskStopNetwork()
{
    mainTask = false;
    sending = false;
    receiving = false;

    outgoingCondition.notify_all();
    incomingCondition.notify_all();

    if (networkThread.joinable())
        networkThread.join();

    if (sendThread.joinable())
        sendThread.join();
    if (receiveThread.joinable())
        receiveThread.join();
}

bool NetworkManager::isRunning() const
{
    return mainTask;
}

Packet NetworkManager::parsePacket(const ENetPacket *enetPacket, ENetPeer *sourcePeer)
{
    if (enetPacket->dataLength < 1)
    {
        logger->console_log("Failed to parse a packet from " + uint32_to_ipv4(sourcePeer->address.host) + ":" + std::to_string(sourcePeer->address.port));

        return Packet(PacketType::PARSING_ERROR, sourcePeer);
    }

    PacketType type = static_cast<PacketType>(enetPacket->data[0]);                             // First byte is the packet type
    std::vector<uint8_t> data(enetPacket->data + 1, enetPacket->data + enetPacket->dataLength); // Remaining data
    return Packet(type, data, sourcePeer);
}

ENetPacket *NetworkManager::createENetPacket(const Packet &packet)
{
    std::vector<uint8_t> rawData = packet.toRawPacket();
    return enet_packet_create(rawData.data(), rawData.size(), ENET_PACKET_FLAG_RELIABLE);
}

void NetworkManager::TaskNetwork()
{
    while (mainTask)
    {
        ENetEvent event;

        while (enet_host_service(host, &event, (1000 / network_frequency)) > 0)
            processENetEvent(event);
    }
    enet_host_destroy(host);
}

void NetworkManager::TaskSend()
{
    while (sending)
    {
        sendOutgoingPackets();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / network_frequency));
    }
}

void NetworkManager::TaskReceive()
{
    while (receiving)
    {
        processIncomingPackets();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / network_frequency));
    }
}

void NetworkManager::sendOutgoingPackets()
{
    while (!outgoingPackets.empty())
    {
        // std::cout << "Sending Packet\n";
        Packet packet = outgoingPackets.front();
        outgoingPackets.pop();
        ENetPacket *enetPacket = createENetPacket(packet);

        if (packet.getPeer())
        {
            enet_peer_send(packet.getPeer(), 0, enetPacket);
        }
        else
        {
            /*std::vector<uint8_t> rawData = packet.getData();*/
            for (size_t i = 0; i < host->peerCount; ++i)
            {
                // ENetPacket *enetPacket = enet_packet_create(rawData.data(), rawData.size(), ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(&host->peers[i], 0, enetPacket);
            }
        }
        
        //enet_packet_destroy(enetPacket);
    }
}

void NetworkManager::processENetEvent(ENetEvent &event)
{
    switch (event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
    {
        onPeerConnect(event.peer);
        break;
    }

    case ENET_EVENT_TYPE_RECEIVE:
    {
        Packet packet = parsePacket(event.packet, event.peer);
        handlePacket(packet, event.peer);
        enet_packet_destroy(event.packet);
        break;
    }

    case ENET_EVENT_TYPE_DISCONNECT:
    {
        onPeerDisconnect(event.peer);
        break;
    }
    default:
    {
        std::cout << "[ENET] EventType = " << event.type << std::endl;
        break;
    }
    }
}

ENetHost *NetworkManager::getHost() { return host; }

std::vector<ENetPeer *> NetworkManager::getPeers()
{
    std::vector<ENetPeer *> peers;

    if (!host)
        return peers;

    for (size_t i = 0; i < host->peerCount; ++i)
    {
        ENetPeer &p = host->peers[i];

        if (p.state == ENET_PEER_STATE_CONNECTED)
            peers.push_back(&p);
    }

    return peers;
}
