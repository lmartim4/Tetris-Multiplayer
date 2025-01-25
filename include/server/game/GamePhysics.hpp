#pragma once

#include <chrono>
#include "BoardController.hpp"
#include "TetrominoController.hpp"
#include "TetrominoManager.hpp"

class Gravity
{
private:
    static constexpr int kMinGravityTimeMs = 400;      // Minimum gravity time (hard cap)
    static constexpr int kInitialGravityTimeMs = 10000; // Starting gravity time
    static constexpr double kDifficultyRate = 0.2;     // Rate at which difficulty increases (adjustable)

    int gravityTimeMs = kInitialGravityTimeMs; // Current gravity time in milliseconds
    int level = 0;                             // Tracks the current level of difficulty

    std::chrono::steady_clock::time_point lastGravityUpdate = std::chrono::steady_clock::now();

public:
    Gravity() = default;

    int getGravityTimeMs() const { return gravityTimeMs; }

    void increaseGravityDifficulty()
    {
        level++;

        gravityTimeMs = static_cast<int>(kInitialGravityTimeMs * std::exp(-kDifficultyRate * level));

        if (gravityTimeMs < kMinGravityTimeMs)
            gravityTimeMs = kMinGravityTimeMs;
    }

    void updateGravityTime()
    {
        lastGravityUpdate = std::chrono::steady_clock::now();
    }

    bool hasGravityIntervalElapsed()
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastGravityUpdate).count();

        if (elapsedMs >= gravityTimeMs)
        {
            lastGravityUpdate = now;
            return true;
        }
        return false;
    }
};

class GamePhysics
{
private:
    Gravity gravity;

    int countNewLockedTetrominos(std::vector<std::shared_ptr<Tetromino>> tetrominos,
                                 std::shared_ptr<BoardController> boardController,
                                 std::shared_ptr<TetrominoController> tetrominoController);

public:
    void applyGravity(std::shared_ptr<BoardController> boardManager, std::shared_ptr<TetrominoController> tetrominoController, std::shared_ptr<TetrominoManager> tetrominoManager);

    CollisionType applyHardDrop(std::shared_ptr<Tetromino> tetromino, std::shared_ptr<BoardController> boardManager, std::shared_ptr<TetrominoController> tetrominoController, std::shared_ptr<TetrominoManager> tetrominoManager);
    CollisionType applyAction(std::shared_ptr<Tetromino> tetromino, TetrisAction action, std::shared_ptr<BoardController> boardController, std::shared_ptr<TetrominoController> tetrominoController, std::shared_ptr<TetrominoManager> tetrominoManager);

    void onTetrominoColide(std::shared_ptr<Tetromino> tetromino, CollisionType col, TetrisAction action, std::shared_ptr<BoardController> boardController, std::shared_ptr<TetrominoController> tetrominoController, std::shared_ptr<TetrominoManager> tetrominoManager);
    void increaseGravityDifficulty() { gravity.increaseGravityDifficulty(); };
};
