#include <enet/enet.h>
#include "ServerManager.hpp"

ServerManager server;

void printJOINRequest(const Packet &packet)
{
    std::cout << "I got a " << PacketTypeToString(packet.type) << " from " << packet.peer->host->address.host << ":" << packet.peer->host->address.port << std::endl;
    server.send_packet(Packet(PacketType::HEARTBEAT, {0}, packet.peer));
}

int main()
{
    if (enet_initialize() != 0)
    {
        std::cerr << "Failed to initialize ENet." << std::endl;
        return EXIT_FAILURE;
    }
    
    server.registerListener(PacketType::JOIN_REQUEST, printJOINRequest);    

    server.startNetwrokTask();
    //Dont do anything after start network
    return 0;
}
