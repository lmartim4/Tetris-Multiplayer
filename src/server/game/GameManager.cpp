#include "GameManager.hpp"

GameManager::GameManager(ServerManager &server) : Debuggable("GameManager"), server(server)
{
    console_log("GameManager()");
}

void GameManager::enqueueAction(Player *player, TetrisAction action)
{
    console_log(std::to_string(player->getPlayerID()) + TetrisActionToString(action));
}

void GameManager::StartGameListener(const Packet &)
{
    server.broadcast_starting_game();
    game = new Game();
    game->startGameLoop();
}

GameManager::~GameManager()
{
}