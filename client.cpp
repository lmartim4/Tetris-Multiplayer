#include <enet/enet.h>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    // Initialize ENet
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet.\n";
        return EXIT_FAILURE;
    }

    // Create a client instance
    ENetHost *client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!client) {
        std::cerr << "An error occurred while trying to create an ENet client host.\n";
        enet_deinitialize();
        return EXIT_FAILURE;
    }

    // Define the server's address (localhost and port 1234)
    ENetAddress address;
    ENetPeer *peer;
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 1234;

    // Connect to the server
    peer = enet_host_connect(client, &address, 2, 0);
    if (!peer) {
        std::cerr << "No available peers for initiating an ENet connection.\n";
        enet_host_destroy(client);
        enet_deinitialize();
        return EXIT_FAILURE;
    }

    // Wait up to 5 seconds for the connection attempt to succeed.
    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connection to 127.0.0.1:1234 succeeded.\n";
    } else {
        // Either the 5 seconds are up or a disconnect event was received.
        std::cerr << "Connection to 127.0.0.1:1234 failed.\n";
        enet_peer_reset(peer);
        enet_host_destroy(client);
        enet_deinitialize();
        return EXIT_FAILURE;
    }

    // Send a packet to the server
    std::string message = "Hello from ENet client!";
    ENetPacket *packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(client);

    // Wait for a response from the server
    while (enet_host_service(client, &event, 5000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "Received packet from server: " << event.packet->data << std::endl;
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnected from server.\n";
                break;
        }
    }

    // Cleanup
    enet_peer_disconnect(peer, 0);
    while (enet_host_service(client, &event, 3000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnection acknowledged.\n";
                break;
        }
    }

    // Destroy the client and deinitialize ENet
    enet_host_destroy(client);
    enet_deinitialize();

    return EXIT_SUCCESS;
}
