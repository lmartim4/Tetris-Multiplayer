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

#include "PacketSender.hpp"
#include "LevelData.hpp"

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
    /* General Game State*/

    TetrisBoard board;
    std::unique_ptr<Tetromino> currentTetromino;

    LevelData levelData;

    PacketSender *packetSender;

    static int instanceCount;
    const int this_instance;

    std::atomic<GameState> gameState = INITIALIZING;
    std::vector<Player *> players;

    int calculateLinesToPoints(int nLines, int level);

    std::thread gameThread; // The thread running the game loop

    void loop();
    void updateGame(TetrisAction action);
    void lockTetromino();
    void broadcastBoardIfChanges() const;
    void spawnTetromino();

public:
    Game(PacketSender *sender);
    ~Game();

    void addPlayer(Player *player);
    void handleInput(Player *player, TetrisAction action);

    void startGameLoop();
    void endGameLoop();

    void clearFullLines();
};