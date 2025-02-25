#include <enet/enet.h>
#include <iostream>

#include "GameManager.hpp"
#include "TetrisAction.hpp"

std::atomic<bool> serverRunning{true};
ServerManager server(12345);
GameManager gm(server);

void onReceiveArrow(const Packet &packet)
{
    Player *player = ServerManager::getPlayerFromPacket(packet);
    TetrisAction action = getActionFromPacketType(packet.getType());

    player->enqueueAction(action);
}

void HeartbeatListener(const Packet &packet)
{
    server.sendPacket(packet);
}

void ReceivePauseRequest(const Packet &packet)
{
    server.broadcastSound(SoundType::DenyErrorSound);
}

void StartGameListener(const Packet &packet)
{
    gm.StartGameListener(packet);
}

void JoinRequestListener(const Packet &packet)
{
    /*
    if (server.getHost()->connectedPeers < 4)
        server.sendPacket(Packet(PacketType::JOIN_ACCEPTED, packet.getPeer()));
    else
        server.sendPacket(Packet(PacketType::JOIN_DENIED, packet.getPeer()));
        */

    server.sendPacket(Packet(PacketType::JOIN_ACCEPTED, server.getPlayerFromPacket(packet)->getData(), packet.getPeer()));
}

void commandHandler()
{
    std::string command;
    while (serverRunning)
    {
        std::getline(std::cin, command);
        if (command == "exit")
        {
            std::cout << "Shutting down server...\n";
            serverRunning = false; // Signal the server to stop
            break;
        }
        else if (command == "restart")
        {
            gm.restart();
        }
        else if (command == "end")
        {
            gm.endGame();
        }
        else if (command == "start")
        {
            gm.start();
        }
    }
}

int main(int argc, const char *argv[])
{
    std::thread commandThread(commandHandler);

    if (server.isRunning())
        std::cout << "Waiting connections...\n";
    else
        return -1;

    server.registerListener(PacketType::PAUSE, ReceivePauseRequest);
    server.registerListener(PacketType::JOIN_REQUEST, JoinRequestListener);
    server.registerListener(PacketType::HEARTBEAT, HeartbeatListener);
    server.registerListener(PacketType::REQUEST_START, StartGameListener);

    server.registerListener(PacketType::LEFT, onReceiveArrow);
    server.registerListener(PacketType::RIGHT, onReceiveArrow);
    server.registerListener(PacketType::ROTATE_CCW, onReceiveArrow);
    server.registerListener(PacketType::ROTATE_CW, onReceiveArrow);
    server.registerListener(PacketType::DROP_FASTER, onReceiveArrow);
    server.registerListener(PacketType::HARD_DROP, onReceiveArrow);

    // Main server loop
    while (serverRunning && server.isRunning())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    gm.endGame();

    if (server.isRunning())
        server.stop();

    if (commandThread.joinable())
        commandThread.join();

    std::cout << "Server stopped. Exiting program.\n";
    return 0;
}
