#include "game/gameManager.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm> // for std::max, std::min

#define P(x) ((280 * (x * x * x) - 1470 * (x * x) + 2630 * (x) - 1320) / 3)
#define LEVEL_UP 10
#define DOWN_FASTER 2 // Define this appropriately if not defined elsewhere

GameManager::GameManager()
    : board(),
      isRunning(false),
      score(0),
      level(0),
      nLinesClearedThisLevel(0),
      levelUpGravityTimeMs(100)
{
    // Initialize gravityTimeMs to a default, for example 800ms:
    gravityTimeMs = 800;
}

void GameManager::startGameLoop()
{
    this->isRunning = true;
    srand((unsigned)time(NULL));
    spawnTetromino();

    // Initialize lastGravityTick
    lastGravityTick = std::chrono::steady_clock::now();

    while (isRunning)
    {
        board.handleInput(*currentTetromino);
        update();
        board.render();

        // Sleep a bit to avoid busy-waiting and allow input handling at ~60 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void GameManager::spawnTetromino()
{
    currentTetromino = TetrominoFactory::createTetromino();
}

int GameManager::lines2Points(int nLines)
{
    return (nLines > 0) ? P(std::min(nLines, 4)) * (level + 1) : 0;
}

void GameManager::update()
{
    if (board.reachedTop())
    {
        std::cout << "(GAME OVER) !!!" << std::endl;
        std::cout << "(Final score) : " << score << " !!!" << std::endl;
        std::cout << "You played till level: " << level << std::endl;

        board.clear();
        exit(0);
    }

    // Clear old tetrominos
    board.clearFallingTetrominos();

    // Check if it's time to apply gravity
    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastGravityTick).count() > gravityTimeMs)
    {
        currentTetromino->dropGravity();
        lastGravityTick = now;
    }

    int lastMove = currentTetromino->getLastMove();

    // Check collision after gravity or moves
    if (board.checkCollision(*currentTetromino))
    {
        if (lastMove == DOWN_FASTER || currentTetromino->getGravity())
        {
            // Place and lock tetromino
            board.placeTetromino(*currentTetromino, true);
            currentTetromino->updateStates();
            currentTetromino.reset();

            nLinesClearedThisLevel += board.clearLines();
            score += lines2Points(nLinesClearedThisLevel);

            if (nLinesClearedThisLevel >= LEVEL_UP)
            {
                level++;
                // Clear all fixed blocks
                board.clearFalledTetrominos();

                gravityTimeMs = std::max(gravityTimeMs - levelUpGravityTimeMs, minTimeMs);
                lastGravityTick = std::chrono::steady_clock::now();
                nLinesClearedThisLevel = 0;
                std::cout << "(Current score) : " << score << " !!!" << std::endl;
                std::cout << "(LEVEL UP) !!!" << std::endl;
            }

            // Spawn a new tetromino
            spawnTetromino();
        }
        else
        {
            // Invalid move, revert
            board.placeTetromino(*currentTetromino, false);
            currentTetromino->updateStates();
        }
    }
    else
    {
        // Valid move, place without locking
        board.placeTetromino(*currentTetromino, false);
        currentTetromino->updateStates();
    }
}
