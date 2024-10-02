#include "NetworkManager.hpp"

// Constructor
NetworkManager::NetworkManager() : running(true) {}

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
void NetworkManager::registerListener(PacketType packetType, std::function<void(const std::vector<uint8_t> &)> callback)
{
    listeners[(uint8_t)packetType] = callback;
}

// Handle a received packet by triggering the corresponding listener
void NetworkManager::handlePacket(const Packet &packet)
{
    
    // Check if there's a listener registered for this packet type
    auto it = listeners.find((int)packet.type);
    if (it != listeners.end())
    {
        // Trigger the registered listener
        it->second(packet.data);
    }
    else
    {
        std::cout << "No listener registered for packet type: " << PacketTypeToString((PacketType)packet.type) << std::endl;
    }
}

// Process incoming packets
void NetworkManager::processIncomingPackets()
{
    while (!incomingPackets.empty())
    {
        Packet packet = incomingPackets.front();
        incomingPackets.pop();
        handlePacket(packet); // Use the new handlePacket function
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
