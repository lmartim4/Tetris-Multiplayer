#pragma once

#include <iostream>
#include "NetworkManager.hpp"
#include <SFML/Graphics.hpp>
#include "AudioManager.hpp"
#include "PlayerData.hpp"
#include "EndGameData.hpp"
#include "PlayerList.hpp"
#include "Tetromino.hpp"
#include <ThreadSafeQueue.hpp>
#include <thread>
#include <atomic>
#include <mutex>
#include "GameStatus.hpp"

class ClientManager : public NetworkManager
{
private:
    bool debugEnabled = false;
    ENetPeer *serverPeer;
    std::atomic<bool> isConnected = false; // Client Connected to Server

    PlayerData me;

    GameStatus score;
    PlayerList players; // Players in the match

    std::mutex nextTetrominoMutex;
    std::mutex getIDMutex;

    ThreadSafeQueue<nlohmann::json> boardBuffer;       // Buffer for received boards (always just the last one)
    ThreadSafeQueue<nlohmann::json> endGameDataBuffer; // Buffer for received end game information (always just the last one)

    std::thread ThreadHeartbeat;
    std::atomic<bool> HeartBeatRunningFlag = false;
    unsigned long last_heartbeat;

protected:
    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;

public:
    ClientManager() {};
    ~ClientManager() {};

    const GameStatus &getGameData() const { return score; }

    void toggleDebug();

    void on_receive_heartbeat();
    void on_receive_score(const Packet &packet);
    void on_receive_player_list(const Packet &packet);
    void on_receive_game_screen(const Packet &packet);
    void on_receive_end_screen(const Packet &packet);

    void on_receive_next_tetromino(const Packet &packet);
    void on_receive_player_id(const Packet &packet);
    void on_receive_background_sound(const Packet &packet);

    void TaskStartHeartbeat();
    void TaskHeartbeat();
    void TaskStopHeartbeat();

    void connect(const std::string &serverAddress = "localhost", uint16_t port = 12345);
    void disconnect();

    bool IsConnected() const { return isConnected; };

    PlayerList &getPlayerList() { return players; }

    bool hasBoard(nlohmann::json &board);
    bool hasEndGameData(nlohmann::json &board);

    ThreadSafeQueue<std::shared_ptr<Tetromino>> next_tetrominos;

    void request_game_start();

    void onPressKey(sf::Event::KeyEvent e);

    int getMyID()
    {
        std::lock_guard<std::mutex> lock(getIDMutex);
        return me.id;
    }

    static void printMatrix(const std::vector<std::vector<int>> &matrix, const std::string &label)
    {
        std::cout << label << ":\n";
        for (const auto &row : matrix)
        {
            for (const auto &cell : row)
                std::cout << cell << " ";
            std::cout << "\n";
        }
        std::cout << std::endl;
    }
};