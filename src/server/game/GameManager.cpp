#include "GameManager.hpp"

GameManager::GameManager(ServerManager &server) : Debuggable("GameManager"), server(server)
{
    console_log("GameManager()");
}

void GameManager::StartGameListener(const Packet &)
{
    if (game != nullptr)
        if (game->getState() == GameState::ENDED)
        {
            delete game;
            game = new Game(server);
        }

    game = new Game(server);

    for (Player *p : server.getPlayers())
        game->addPlayer(p);

    server.broadcast_starting_game();
    game->startGame();
}

GameManager::~GameManager()
{
}