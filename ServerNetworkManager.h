// ServerNetworkManager.hpp

#ifndef SERVER_NETWORK_MANAGER_HPP
#define SERVER_NETWORK_MANAGER_HPP

#include "NetworkManager.h"

class ServerNetworkManager : public NetworkManager {
private:
    ENetHost* server;
    std::thread networkThread;

public:
    ServerNetworkManager(uint16_t port = 12345) {
        ENetAddress address;
        enet_address_set_host(&address, "0.0.0.0");
        address.port = port;

        server = enet_host_create(&address, 32, 2, 0, 0);  // 32 clients max, 2 channels
        if (!server) {
            throw std::runtime_error("Failed to create ENet server host.");
        }

        networkThread = std::thread(&ServerNetworkManager::networkLoop, this);
    }

    ~ServerNetworkManager() {
        enet_host_destroy(server);
        if (networkThread.joinable()) {
            networkThread.join();
        }
    }

    void networkLoop() {
        while (running) {
            ENetEvent event;
            while (enet_host_service(server, &event, 1000) > 0) {
                switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "Client connected: " << event.peer->address.host << std::endl;
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "Packet received from client." << std::endl;
                    enqueueIncomingPacket(parsePacket(event.packet));
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Client disconnected." << std::endl;
                    break;

                default:
                    break;
                }
            }

            sendOutgoingPackets();
            processIncomingPackets();
        }
    }

    void sendOutgoingPackets() override {
        while (!outgoingPackets.empty()) {
            Packet packet = outgoingPackets.front();
            outgoingPackets.pop();

            for (size_t i = 0; i < server->peerCount; ++i) {
                ENetPacket* enetPacket = createENetPacket(packet);
                enet_peer_send(&server->peers[i], 0, enetPacket);
            }
        }
    }

    Packet parsePacket(ENetPacket* enetPacket) {
        std::vector<uint8_t> data(enetPacket->data, enetPacket->data + enetPacket->dataLength);
        return Packet(data);
    }

    ENetPacket* createENetPacket(const Packet& packet) {
        return enet_packet_create(packet.data.data(), packet.data.size(), ENET_PACKET_FLAG_RELIABLE);
    }
};

#endif // SERVER_NETWORK_MANAGER_HPP
