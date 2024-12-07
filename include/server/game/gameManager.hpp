#pragma once

#include "game/tetrominoFactory.hpp"
#include "board.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

class GameManager
{
private:
    TetrisBoard board;
    std::unique_ptr<Tetromino> currentTetromino;

    bool isRunning;
    int score;
    int level;
    int nLinesClearedThisLevel;

    const int minTimeMs = 400;
    int gravityTimeMs;          // Current gravity time in ms
    int levelUpGravityTimeMs;   // How much to reduce gravity time on level up

    std::chrono::steady_clock::time_point lastGravityTick; // To track gravity intervals

public:
    GameManager();

    int lines2Points(int nLines); 
    void startGameLoop();

    void spawnTetromino();

    void update();
};
