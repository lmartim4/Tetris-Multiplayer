#pragma once

#include "Game.hpp"
#include "ServerManager.hpp"
#include "Logger.hpp"
#include "TetrisAction.hpp"

class GameManager
{
private:
    Logger *logger;
    ServerManager &server;

    Game *game;

public:
    void StartGameListener(const Packet &);
    void restart();
    void endGame();
    void start();
    GameManager(ServerManager &server);
    ~GameManager();
};
