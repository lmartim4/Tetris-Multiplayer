#include <enet/enet.h>
#include "ServerManager.hpp"

ServerManager server;

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

int main()
{
    server.registerListener(PacketType::JOIN_REQUEST, JoinRequestListener);
    server.registerListener(PacketType::HEARTBEAT, HeartbeatListener);

    

    // Do not run anything after this
    server.startNetwrokTask();
    // Dont do anything after start network
    return 0;
}
