#include <enet/enet.h>
#include <iostream>
#include "ServerManager.hpp"

ServerManager server(12345);

void HeartbeatListener(const Packet &packet)
{
    server.send_packet(packet);
}

void JoinRequestListener(const Packet &packet)
{
    if (server.getHost()->connectedPeers < 4)
        server.send_packet(Packet(PacketType::JOIN_ACCEPTED, {0}, packet.peer));
    else
        server.send_packet(Packet(PacketType::JOIN_DENIED, {0}, packet.peer));
}

int main(int argc, const char *argv[])
{
    server.network_print("Waiting connections...\n");

    server.registerListener(PacketType::JOIN_REQUEST, JoinRequestListener);
    server.registerListener(PacketType::HEARTBEAT, HeartbeatListener);

    while (server.isRunning())
        ;
    return 0;
}
