#pragma once

#include <iostream>
#include "NetworkManager.hpp"
#include <SFML/Graphics.hpp>
#include "AudioManager.hpp"
#include "PlayerData.hpp"
#include "EndGameData.hpp"
#include "PlayerList.hpp"
#include <ThreadSafeQueue.hpp>
#include <thread>
#include <atomic>
#include <mutex>

class ClientManager : public NetworkManager
{
private:
    bool debugEnabled = false;
    ENetPeer *serverPeer;
    std::atomic<bool> isConnected = false; // Client Connected to Server

    PlayerList players; // Players in the match

    ThreadSafeQueue<nlohmann::json> boardBuffer;       // Buffer for received boards (always just the last one)
    ThreadSafeQueue<nlohmann::json> endGameDataBuffer; // Buffer for received end game information (always just the last one)

    std::thread ThreadHeartbeat;
    std::atomic<bool> HeartBeatRunningFlag = false;
    unsigned long last_heartbeat;

    AudioManager &audio;

protected:
    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;

public:
    ClientManager(AudioManager &audioManager) : audio(audioManager) {};
    ~ClientManager() {};

    void toggleDebug();

    void on_receive_heartbeat();
    void on_receive_player_list(const Packet &packet);
    void on_receive_game_screen(const Packet &packet);
    void on_receive_end_screen(const Packet &packet);
    void on_receive_play_sound(const Packet &packet);

    void TaskStartHeartbeat();
    void TaskHeartbeat();
    void TaskStopHeartbeat();

    void connect(const std::string &serverAddress = "127.0.0.1", uint16_t port = 12345);
    void disconnect();

    bool IsConnected() const { return isConnected; };
    
    PlayerList &getPlayerList() { return players; }

    bool hasBoard(nlohmann::json &board);
    bool hasEndGameData(nlohmann::json &board);
    void request_game_start();

    void onPressKey(sf::Event::KeyEvent e);
};