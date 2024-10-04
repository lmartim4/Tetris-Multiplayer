#include <enet/enet.h>
#include "ClientManager.hpp"
#include "TetrisActions.hpp"

ClientManager client;

void printHEARTBEAT(const Packet &packet)
{
    client.heartbeat_listener();
}

int main()
{
    client.registerListener(PacketType::HEARTBEAT, printHEARTBEAT);

    while (client.isRunning())
        ;

    return 0;
}
