#include <enet/enet.h>
#include "ClientManager.hpp"
#include "TetrisActions.hpp"

ClientManager client;

void printHEARTBEAT(const Packet &packet)
{
    client.on_receive_heartbeat();
}

int main()
{
    client.registerListener(PacketType::HEARTBEAT, printHEARTBEAT);

    // Do not run anything after this
    client.startNetwrokTask();
    // Dont do anything after start network
    return 0;
}
