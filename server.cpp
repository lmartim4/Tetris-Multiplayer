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

void printJOINRequest(const std::vector<uint8_t> &data)
{
    std::cout << "I got a join request" << std::endl;
}

int main()
{
    if (enet_initialize() != 0)
    {
        std::cerr << "Failed to initialize ENet." << std::endl;
        return EXIT_FAILURE;
    }

    Server server;
    server.serverManager.registerListener(PacketType::JOIN_REQUEST, printJOINRequest);
    server.run();

    enet_deinitialize();
    return 0;
}
