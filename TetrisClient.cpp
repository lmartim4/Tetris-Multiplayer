#include <enet/enet.h>
#include "ClientManager.hpp"
#include "TetrisActions.hpp"

ClientManager client;

void heartbeat_listener(const Packet &packet)
{
    client.on_receive_heartbeat();
}

int main()
{
    client.registerListener(PacketType::HEARTBEAT, heartbeat_listener);

    while (client.isRunning())
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    return 0;
}
