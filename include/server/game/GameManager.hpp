#pragma once

#include "Game.hpp"
#include "ServerManager.hpp"
#include "Debuggable.hpp"
#include "TetrisAction.hpp"

class GameManager : public Debuggable
{
private:
    ServerManager &server;

    Game *game; 

public:
    void StartGameListener(const Packet &);
    void enqueueAction(Player *player, TetrisAction action);

    GameManager(ServerManager &server);
    ~GameManager();
};
