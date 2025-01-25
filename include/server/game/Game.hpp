#pragma once

#include "BoardController.hpp"
#include "EndGameData.hpp"
#include "Player.hpp"
#include "GameStatus.hpp"
#include "GravityManager.hpp"

#include "ThreadSafeQueue.hpp"
#include "Logger.hpp"
#include "ServerManager.hpp"

#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <map>

#define GAME_DEBUG

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

    Logger *logger;

    ServerManager &server;
    std::atomic<GameState> gameState = INITIALIZING;

    GravityManager gravity;

    GameStatus gameData;

    BoardController *boardController;
    std::shared_ptr<TetrisBoard> board;
    std::vector<Player *> players;

    std::map<Player *, std::shared_ptr<Tetromino>> currentTetromino;
    std::map<Player *, std::shared_ptr<Tetromino>> nextTetromino;

    void loop();
    void processIncommingInputs();
    void updateGame(std::shared_ptr<Tetromino> tetromino, TetrisAction action);

    int tryClearFullLines();
    int calculatePoints(int nLines, int level);

    void spawnNextTetromino(Player *player);
    void onTetrominoColide(std::shared_ptr<Tetromino> tetromino, CollisionType col);

    void broadcastBoardIfChanges() const;
    void broadcastEndGameStatus() const;

    int countNewLockedTetrominos(std::vector<std::shared_ptr<Tetromino>> tetrominos);

    void processGravity();

    std::thread gameThread;

public:
    Game(ServerManager &sender);
    ~Game();

    GameState getState() { return gameState; }

    void addPlayer(Player *player);
    void startGame();
    void endGameLoop();
};