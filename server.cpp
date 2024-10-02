#include <enet/enet.h>
#include <iostream>

int main(int argc, char** argv) {
    // Initialize ENet
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet.\n";
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    // Create an ENet server host
    ENetAddress address;
    ENetHost* server;

    address.host = ENET_HOST_ANY;  // Accept connections from any host
    address.port = 1234;           // Use port 1234 for communication

    server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet server host.\n";
        return EXIT_FAILURE;
    }

    std::cout << "Server started. Waiting for connections...\n";

    // Main server loop
    ENetEvent event;
    while (true) {
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "A new client connected from "
                              << event.peer->address.host << ":" << event.peer->address.port << ".\n";
                    event.peer->data = (void*)"Client information";
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "Message received: " << (char*)event.packet->data << "\n";
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Client disconnected.\n";
                    event.peer->data = nullptr;
                    break;
            }
        }
    }

    // Cleanup
    enet_host_destroy(server);
    return 0;
}
