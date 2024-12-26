#include "GameManager.hpp"

GameManager::GameManager(ServerManager &server) : Debuggable("GameManager"), server(server)
{
    console_log("GameManager()");
}

void GameManager::StartGameListener(const Packet &)
{
    if (game != nullptr)
        throw std::runtime_error("Game was already started!");

    game = new Game(server);

    for (Player *p : server.getPlayers())
        game->addPlayer(p);

    server.broadcast_starting_game();
    game->startGameLoop();
}

GameManager::~GameManager()
{
}