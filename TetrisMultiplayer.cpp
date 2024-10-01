#include <enet/enet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to initialize ENet
void initialize_enet() {
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        exit(EXIT_FAILURE);
    }
    atexit(enet_deinitialize); // Ensure ENet is cleaned up on exit
}

// Function to create a client
ENetHost* create_client() {
    ENetHost* client = enet_host_create(NULL, 1, 2, 0, 0);
    if (client == NULL) {
        fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
        exit(EXIT_FAILURE);
    }
    return client;
}

// Function to connect to the server
ENetPeer* connect_to_server(ENetHost* client, const char* server_ip, enet_uint16 port) {
    ENetAddress address;
    ENetEvent event;

    enet_address_set_host(&address, server_ip);
    address.port = port;

    ENetPeer* peer = enet_host_connect(client, &address, 2, 0);
    if (peer == NULL) {
        fprintf(stderr, "No available peers for initiating an ENet connection.\n");
        exit(EXIT_FAILURE);
    }

    // Wait up to 5 seconds for the connection attempt to succeed
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        printf("Connection to server succeeded.\n");
    }
    else {
        enet_peer_reset(peer);
        printf("Connection to server failed.\n");
        exit(EXIT_FAILURE);
    }

    return peer;
}

// Function to send a packet to the server
void send_packet(ENetPeer* peer, const char* message) {
    ENetPacket* packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(peer->host);
}

// Main loop to handle incoming packets
void client_main_loop(ENetHost* client, ENetPeer* peer) {
    ENetEvent event;

    while (1) {
        while (enet_host_service(client, &event, 1000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                printf("A packet of length %zu containing \"%s\" was received from the server.\n",
                    event.packet->dataLength, (char*)event.packet->data);
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf("Disconnected from the server.\n");
                return;

            default:
                break;
            }
        }
    }
}

int main(int argc, char** argv) {
    const char* server_ip = "127.0.0.1";  // Change to your server's IP address if it's on another machine
    enet_uint16 server_port = 1234;       // The port you used for your server

    // Initialize ENet
    initialize_enet();

    // Create the client
    ENetHost* client = create_client();

    // Connect to the server
    ENetPeer* peer = connect_to_server(client, server_ip, server_port);

    // Send a message to the server
    send_packet(peer, "Hello, Server!");

    // Enter the main loop to receive messages from the server
    client_main_loop(client, peer);

    // Clean up and destroy the client host
    enet_host_destroy(client);

    return 0;
}
