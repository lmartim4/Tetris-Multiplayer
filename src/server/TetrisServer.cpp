#include <enet/enet.h>
#include <iostream>
#include "ServerManager.hpp"
#include "TetrisAction.hpp"

ServerManager server(12345);
GameManager gm(server);

// Might get player from packt id
void onReceiveArrow(const Packet &packet)
{
    TetrisAction action = getActionFromPacketType(packet.type);
    gm.enqueueAction(action);
}

void HeartbeatListener(const Packet &packet)
{
    server.send_packet(packet);
}

void StartGameListener(const Packet &packet)
{
    server.start_game();
    gm.startGameLoop();
}

void JoinRequestListener(const Packet &packet)
{
    if (server.getHost()->connectedPeers < 4)
        server.send_packet(Packet(PacketType::JOIN_ACCEPTED, 0, packet.peer));
    else
        server.send_packet(Packet(PacketType::JOIN_DENIED, 0, packet.peer));
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
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    return 0;
}
