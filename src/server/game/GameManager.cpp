#include "GameManager.hpp"

GameManager::GameManager(ServerManager &server) : server(server)
{
    logger = new Logger("GameManager");
    logger->console_log("GameManager()");
}

void GameManager::StartGameListener(const Packet &p)
{

    if (game != nullptr)
    {
        if (game->getState() == GameState::ENDED)
        {
            delete game;
        }
        else
        {
            logger->console_log("Cannot start a match while on is already running. Client should wait until next match!");
            return;
        }
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