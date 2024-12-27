#pragma once

#include "TetrisBoardController.hpp"
#include "EndGameData.hpp"
#include "Player.hpp"
#include "LevelData.hpp"

#include "ThreadSafeQueue.hpp"
#include "Debuggable.hpp"
#include "ServerManager.hpp"

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
    ENDING,          // TELL THREADS TO STOP
    ENDED            // EVERYTHING IS ENDED
};

class Game
{
private:
    static int instanceCount;
    const int this_instance;

    Debuggable *logger;

    ServerManager &server;
    std::atomic<GameState> gameState = INITIALIZING;
    LevelData levelData;

    TetrisBoardController *boardController;
    TetrisBoard board;
    std::vector<Player *> players;
    std::unique_ptr<Tetromino> currentTetromino;

    void loop();
    void processIncommingInputs();
    void updateGame(TetrisAction action);
    void lockTetromino();
    void tryClearFullLines();
    int calculatePoints(int nLines, int level);

    void spawnTetromino();
    void tetrominoHasFallen();

    void broadcastBoardIfChanges() const;
    void broadcastEndGameStatus() const;

    std::thread gameThread;

public:
    Game(ServerManager &sender);
    ~Game();

    void addPlayer(Player *player);
    GameState getState() { return gameState; }
    void startGame();
    void endGameLoop();
};