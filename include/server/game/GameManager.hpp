#pragma once

#include "Game.hpp"
#include "ServerManager.hpp"
#include "Debuggable.hpp"
#include "TetrisAction.hpp"

class GameManager
{
private:
    Debuggable *logger;
    ServerManager &server;

    Game *game;

public:
    void StartGameListener(const Packet &);

    GameManager(ServerManager &server);
    ~GameManager();
};
