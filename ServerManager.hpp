#ifndef SERVER_NETWORK_MANAGER_HPP
#define SERVER_NETWORK_MANAGER_HPP

#include "NetworkManager.hpp"
#include <enet/enet.h>
#include <thread>

class ServerManager : public NetworkManager
{
private:
public:
    ServerManager(uint16_t port = 12345);
    ~ServerManager();
};

#endif // SERVER_NETWORK_MANAGER_HPP
