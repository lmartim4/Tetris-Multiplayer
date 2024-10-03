#include <enet/enet.h>
#include "ClientNetworkManager.hpp"
#include "TetrisActions.hpp"

class Client
{
public:
    Client() : clientManager("localhost", 12345)
    {
        
    }

    void run()
    {
        std::cout << "Client is running..." << std::endl;
        while (clientManager.isRunning())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    ClientNetworkManager clientManager;

private:
};

Client client;

void printHEARTBEAT(const Packet &packet, ENetPeer *peer)
{
    std::cout << "I got a " << PacketTypeToString(packet.type) << " from " << peer->address.host << ":" << peer->address.port << std::endl;
    client.clientManager.sendPacketToPeer(peer, Packet(PacketType::HEARTBEAT, {0}));
    
}

int main()
{
    if (enet_initialize() != 0)
    {
        std::cerr << "Failed to initialize ENet." << std::endl;
        return EXIT_FAILURE;
    }

    client.clientManager.registerListener(PacketType::HEARTBEAT, printHEARTBEAT);
    client.run();

    enet_deinitialize();
    return 0;
}
