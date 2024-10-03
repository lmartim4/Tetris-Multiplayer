#ifndef CLIENT_NETWORK_MANAGER_HPP
#define CLIENT_NETWORK_MANAGER_HPP

#include "NetworkManager.hpp"
#include <SFML/Graphics.hpp>
#include <thread>
#include <atomic>

class ClientManager : public NetworkManager
{
private:
    ENetPeer *serverPeer;
    std::thread actionThread;
    std::thread windowThread;
    bool isConnected = false;
    unsigned long last_heartbeat;
    std::atomic<bool> running;
    bool debugEnabled = false;

public:
    ClientManager(const std::string &serverAddress = "localhost", uint16_t port = 12345);
    ~ClientManager();

    void HEARTBEAT_TASK();

    void on_receive_heartbeat()
    {
        // Get the current time point
        auto now = std::chrono::system_clock::now();

        // Convert the time point to milliseconds since epoch
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        last_heartbeat = now_ms;
    }

    void windowLoop(); // Função principal do loop da janela

    void stopWindow(); // Para parar a thread da janela

    // Método para alternar o estado de debug
    void toggleDebug();
};

#endif // CLIENT_NETWORK_MANAGER_HPP
