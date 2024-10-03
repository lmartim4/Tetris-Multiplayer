#include <enet/enet.h>
#include "ClientManager.hpp"
#include "TetrisActions.hpp"

ClientManager client("localhost", 12345);

void printHEARTBEAT(const Packet &packet)
{
    std::cout << "I got a " << PacketTypeToString(packet.type) << " from " << packet.peer->host->address.host << std::endl;
    // client.sendPacketToPeer(peer, Packet(PacketType::HEARTBEAT, {0}));
}

int main()
{
    if (enet_initialize() != 0)
    {
        std::cerr << "Failed to initialize ENet." << std::endl;
        return EXIT_FAILURE;
    }

    client.registerListener(PacketType::HEARTBEAT, printHEARTBEAT);

    client.startNetwrokTask();
    //Dont do anything after start network
    return 0;
}
