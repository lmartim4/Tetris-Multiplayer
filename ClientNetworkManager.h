// ClientNetworkManager.hpp

#ifndef CLIENT_NETWORK_MANAGER_HPP
#define CLIENT_NETWORK_MANAGER_HPP

#include "NetworkManager.h"

class ClientNetworkManager : public NetworkManager {
private:
    ENetHost* client;
    ENetPeer* peer;
    std::thread networkThread;

public:
    ClientNetworkManager(const std::string& serverAddress = "localhost", uint16_t port = 12345) {
        client = enet_host_create(nullptr, 1, 2, 0, 0);  // 1 client, 2 channels
        if (!client) {
            throw std::runtime_error("Failed to create ENet client host.");
        }

        ENetAddress address;
        enet_address_set_host(&address, serverAddress.c_str());
        address.port = port;

        peer = enet_host_connect(client, &address, 2, 0);
        if (!peer) {
            throw std::runtime_error("No available peers for initiating an ENet connection.");
        }

        networkThread = std::thread(&ClientNetworkManager::networkLoop, this);
    }

    ~ClientNetworkManager() {
        enet_host_destroy(client);
        if (networkThread.joinable()) {
            networkThread.join();
        }
    }

    void networkLoop() {
        while (running) {
            ENetEvent event;
            while (enet_host_service(client, &event, 1000) > 0) {
                switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "Connected to server." << std::endl;
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "Packet received from server." << std::endl;
                    enqueueIncomingPacket(parsePacket(event.packet));
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Disconnected from server." << std::endl;
                    peer = nullptr;
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

            ENetPacket* enetPacket = createENetPacket(packet);
            enet_peer_send(peer, 0, enetPacket);
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

#endif // CLIENT_NETWORK_MANAGER_HPP
