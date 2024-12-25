#pragma once
#include "PacketType.hpp"

struct PacketSender
{
    virtual ~PacketSender() = default;
    
    // Send a packet to a specific destination
    virtual void sendPacket(const Packet &packet) = 0;
};
