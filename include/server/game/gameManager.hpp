#pragma once

#include "game/tetrominoFactory.hpp"
#include "ServerManager.hpp"
#include "board.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>
#include <atomic>

class GameManager
{
private:
    TetrisBoard board;
    std::unique_ptr<Tetromino> currentTetromino;

    std::atomic<bool> isRunning;
    int score;
    int level;
    int nLinesClearedThisLevel;

    const int minTimeMs = 400;
    int gravityTimeMs;          // Current gravity time in ms
    int levelUpGravityTimeMs;   // How much to reduce gravity time on level up

    std::chrono::steady_clock::time_point lastGravityTick; // To track gravity intervals

    std::thread gameThread;     // The thread running the game loop
    std::atomic<bool> threadActive; // To track if the thread is active

    void runGameLoop();
    
public:
    GameManager(ServerManager &serverManager);

    int lines2Points(int nLines);

    // Starts the game loop in a separate thread
    void startGameLoop();

    // Stops the game loop and waits for the thread to finish
    void stopGameLoop();

    // Checks if the game is currently running
    bool isGameRunning() const;

    // Checks if the game thread is running
    bool isThreadRunning() const;

    void spawnTetromino();
    void update();
};
