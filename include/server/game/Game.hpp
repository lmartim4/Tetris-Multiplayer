#pragma once

#include "Debuggable.hpp"

#include "ThreadSafeQueue.hpp"
#include "Player.hpp"
#include "TetrisBoard.hpp"
#include "EndGameData.hpp"

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
    ENDING,          // TELL THREADS TO STOP
    ENDED            // EVERYTHING IS ENDED
};

class Game : public Debuggable
{
private:
    static int instanceCount;
    const int this_instance;

    PacketSender *packetSender;
    std::atomic<GameState> gameState = INITIALIZING;
    LevelData levelData;

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
    void broadcastBoardIfChanges() const;
    void broadcastEndGameStatus() const;

    std::thread gameThread; // The thread running the game loop
public:
    Game(PacketSender *sender);
    ~Game();
    
    void addPlayer(Player *player);
    
    void startGameLoop();
    void endGameLoop();
};