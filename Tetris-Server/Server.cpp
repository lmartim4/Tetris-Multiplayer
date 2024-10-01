#include <enet/enet.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    // Initialize ENet
    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    // Create a host (server)
    ENetAddress address;
    ENetHost* server;

    address.host = ENET_HOST_ANY; // Bind to any available network interface
    address.port = 1234;          // The port you want the server to listen on

    server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == NULL)
    {
        fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
        exit(EXIT_FAILURE);
    }
    printf("ENet server started. Listening on port %d...\n", address.port);

    // Server Loop
    ENetEvent event;
    while (1)
    {
        while (enet_host_service(server, &event, 1000) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n",
                    event.peer->address.host,
                    event.peer->address.port);
                // Store any data you wish associated with this peer connection
                //event.peer->data = "Client information";
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                printf("A packet of length %zu was received from %s on channel %u.\n",
                    event.packet->dataLength,
                    (char*)event.peer->data,
                    event.channelID);

                // Process the packet data here if needed
                enet_packet_destroy(event.packet); // Clean up after handling the packet
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf("%s disconnected.\n", (char*)event.peer->data);
                // Reset the peer's data
                event.peer->data = NULL;
                break;

            default:
                break;
            }
        }
    }

    // Clean up and destroy the host
    enet_host_destroy(server);

    return 0;
}
