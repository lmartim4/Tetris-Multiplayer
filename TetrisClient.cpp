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
        ;

    return 0;
}
