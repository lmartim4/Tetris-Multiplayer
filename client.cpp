// client_main.cpp

#include <enet/enet.h>
#include "ClientNetworkManager.h"

class Client {
public:
    Client() : clientManager("localhost", 12345) {}

    void run() {
        std::cout << "Client is running..." << std::endl;
        while (clientManager.isRunning()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

private:
    ClientNetworkManager clientManager;
};

int main() {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet." << std::endl;
        return EXIT_FAILURE;
    }

    Client client;
    client.run();

    enet_deinitialize();
    return 0;
}
