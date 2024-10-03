#include <enet/enet.h>
#include "ServerNetworkManager.hpp"

class Server
{
public:
    Server() : serverManager(12345) {}

    void run()
    {
        std::cout << "Server is running..." << std::endl;
        while (serverManager.isRunning())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    ServerNetworkManager serverManager;

private:
};


Server server;

void printJOINRequest(const Packet &packet, ENetPeer *peer)
{
    std::cout << "I got a " << PacketTypeToString(packet.type) << " from " << peer->address.host << ":" << peer->address.port << std::endl;
    server.serverManager.sendPacketToPeer(peer, Packet(PacketType::HEARTBEAT, {0}));
    
}

int main()
{
    if (enet_initialize() != 0)
    {
        std::cerr << "Failed to initialize ENet." << std::endl;
        return EXIT_FAILURE;
    }
    server.serverManager.registerListener(PacketType::JOIN_REQUEST, printJOINRequest);
    server.run();
    enet_deinitialize();
    return 0;
}
