#pragma once

#include "Debuggable.hpp"

#include "ThreadSafeQueue.hpp"
#include "Player.hpp"
#include "TetrisBoard.hpp"
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <map>

enum GameState
{
    INITIALIZING,    // WAITING CLASS CREATION
    WAITING_PLAYERS, // SETTING UP PLAYER LIST
    STARTING,        // STARTING THREADS (not running yet)
    RUNNNING,        // THREADS ARE RUNNING
    ENDING,          // THREADS ARE STOPING
    ENDED            // EVERYTHING IS ENDED
};

class Game : public Debuggable
{
private:
    static int instanceCount;
    const int this_instance;

    std::atomic<GameState> gameState = INITIALIZING;
    std::vector<Player *> players;

    ThreadSafeQueue<TetrisAction> actionQueue;

    TetrisBoard board;

    int calculateLinesToPoints(int nLines, int level);

    void handleInput(TetrisAction action);

    std::thread gameThread; // The thread running the game loop
    void loop();

public:
    Game();
    ~Game();

    void addPlayer(Player *player);

    void enqueueAction(TetrisAction action);

    void startGameLoop();
    void endGameLoop();
};