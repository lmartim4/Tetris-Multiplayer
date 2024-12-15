#include <enet/enet.h>
#include <iostream>

#include "GameManager.hpp"
#include "TetrisAction.hpp"

ServerManager server(12345);
GameManager gm(server);

void onReceiveArrow(const Packet &packet)
{
    std::cout << "I got a packet!" << std::endl;
    TetrisAction action = getActionFromPacketType(packet.type);

    Player *player = ServerManager::extractPlayerFromPacket(packet);

    gm.enqueueAction(player, action);
}

void HeartbeatListener(const Packet &packet)
{
    server.sendPacket(packet);
}

void StartGameListener(const Packet &packet)
{
    gm.StartGameListener(packet);
}

void JoinRequestListener(const Packet &packet)
{
    if (server.getHost()->connectedPeers < 4)
        server.sendPacket(Packet(PacketType::JOIN_ACCEPTED, 0, packet.peer));
    else
        server.sendPacket(Packet(PacketType::JOIN_DENIED, 0, packet.peer));
}

int main(int argc, const char *argv[])
{
    if (server.isRunning())
        server.network_print("Waiting connections...\n");
    else
        return -1;

    server.registerListener(PacketType::JOIN_REQUEST, JoinRequestListener);
    server.registerListener(PacketType::HEARTBEAT, HeartbeatListener);
    server.registerListener(PacketType::REQUEST_START, StartGameListener);

    server.registerListener(PacketType::LEFT, onReceiveArrow);
    server.registerListener(PacketType::RIGHT, onReceiveArrow);
    server.registerListener(PacketType::ROTATE_LEFT, onReceiveArrow);
    server.registerListener(PacketType::ROTATE_RIGHT, onReceiveArrow);
    server.registerListener(PacketType::DROP_FASTER, onReceiveArrow);
    server.registerListener(PacketType::DROP_INSTANT, onReceiveArrow);

    while (server.isRunning())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    return 0;
}
