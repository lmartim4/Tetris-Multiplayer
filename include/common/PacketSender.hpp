#pragma once
#include "PacketType.hpp"

struct PacketSender
{
    virtual ~PacketSender() = default;
    
    // Send a packet to a specific destination
    virtual void sendPacket(const Packet &packet) = 0;

    // Broadcast a packet to all connected peers
    virtual void broadcastPacket(const Packet &packet) = 0;
};
