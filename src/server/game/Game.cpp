#include "Game.hpp"

int Game::instanceCount = 0;

void Game::enqueueAction(TetrisAction action)
{
    // actionQueue.push(action);
    console_log("received action: " + TetrisActionToString(action));
}

void Game::startGameLoop()
{
    static int instanceCount = 0;
    if (gameState != GameState::WAITING_PLAYERS)
    {
        std::cerr << "Cannot start a game that is not currently waiting for players" << std::endl;
        return;
    }

    gameState = GameState::STARTING;
    console_log("Starting Game (" + std::to_string(instanceCount++) + ")...");

    gameThread = std::thread(&Game::loop, this);
}

void Game::endGameLoop()
{
    if (gameState != GameState::RUNNNING)
    {
        console_log("Cannot end a game that is not currently running");
        return;
    }

    gameState = GameState::ENDING;

    if (gameThread.joinable())
        gameThread.join();

    if (gameState == GameState::ENDED)
    {
        console_log("Successfully ended gameLoop");
    }
    else
    {
        throw std::runtime_error("Failed to end game Loop");
    }
}

int Game::calculateLinesToPoints(int nLines, int level)
{
    int x = std::min(nLines, 4);
    int P_x = ((280 * (x * x * x) - 1470 * (x * x) + 2630 * (x)-1320) / 3);

    return (nLines > 0) ? P_x * (level + 1) : 0;
}

void Game::handleInput(TetrisAction action)
{
}

void Game::loop()
{
    gameState = GameState::RUNNNING;

    game

    while (gameState != GameState::ENDING)
    {

    }

    gameState = GameState::ENDED;
}

Game::Game() : Debuggable("Game"), this_instance(instanceCount++)
{
    console_log("Initializing Game (" + std::to_string(this_instance) + ")...");
    gameState = GameState::WAITING_PLAYERS;
}

Game::~Game()
{
    console_log("Destroying Game (" + std::to_string(this_instance) + ")...");
}

void Game::addPlayer(Player *player)
{
    if (gameState != GameState::WAITING_PLAYERS)
    {
        console_log("Cannot add player (not waiting for players)");
        return;
    }

    players.emplace_back(player);
}
