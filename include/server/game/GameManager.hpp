#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <memory>
#include <atomic>

#include "ThreadSafeQueue.hpp"
#include "game/tetrominoFactory.hpp"
#include "TetrisAction.hpp"
#include "ServerManager.hpp"
#include "TetrisBoard.hpp"
#include <mutex>

class GameManager
{
private:
    std::mutex gameStateMutex;
    ThreadSafeQueue<TetrisAction> actionQueue;

    TetrisBoard board;
    std::unique_ptr<Tetromino> currentTetromino;

    std::atomic<bool> isRunning;
    int score;
    int level;
    int nLinesClearedThisLevel;

    const int minTimeMs = 400;
    int gravityTimeMs;        // Current gravity time in ms
    int levelUpGravityTimeMs; // How much to reduce gravity time on level up

    std::chrono::steady_clock::time_point lastGravityTick; // To track gravity intervals

    std::thread gameThread;         // The thread running the game loop
    std::atomic<bool> threadActive; // To track if the thread is active

    TetrisAction lastM = TetrisAction::EMPTY;

    void handleInput(TetrisAction action);
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

    void enqueueAction(TetrisAction action)
    {
        actionQueue.push(action);
    }
};
