#include <enet/enet.h>
#include "ClientNetworkManager.hpp"
#include "TetrisActions.hpp"

// Example listener function for Tetris actions
void onTetrisActionReceived(const std::vector<uint8_t> &data)
{
    if (data.empty())
        return;

    TetrisAction action = static_cast<TetrisAction>(data[0]);
    std::cout << "Listener received Tetris action: " << TetrisActionToString(action) << std::endl;
}

class Client
{
public:
    Client() : clientManager("localhost", 12345)
    {
        // Register listener for Tetris actions
        for (uint8_t i = 0; i < 10; ++i)
        {
            clientManager.registerListener((PacketType)i, onTetrisActionReceived);
        }
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

int main()
{
    if (enet_initialize() != 0)
    {
        std::cerr << "Failed to initialize ENet." << std::endl;
        return EXIT_FAILURE;
    }

    Client client;
    client.run();

    enet_deinitialize();
    return 0;
}
