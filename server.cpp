// server_main.cpp

#include <enet/enet.h>
#include "ServerNetworkManager.h"

class Server {
public:
    Server() : serverManager(12345) {}

    void run() {
        std::cout << "Server is running..." << std::endl;
        while (serverManager.isRunning()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

private:
    ServerNetworkManager serverManager;
};

int main() {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet." << std::endl;
        return EXIT_FAILURE;
    }

    Server server;
    server.run();

    enet_deinitialize();
    return 0;
}
