#pragma once

#include <chrono>

class LevelData
{
private:
    // Basic level/score data
    int score_ = 0;
    int level_ = 1;
    int totalLinesCleared = 0;

    int linesClearedThisLevel_ = 0;

    // Gravity-related data
    static constexpr int kMinGravityTimeMs = 400; // Minimum gravity time
    int gravityTimeMs_ = 500;                     // Current gravity time
    int gravityDecrementPerLevel_ = 50;           // How much to reduce per level
    std::chrono::steady_clock::time_point lastGravityUpdate_ = std::chrono::steady_clock::now();

public:
    // Empty constructor (uses default member initializers)
    LevelData() = default;

    // Check and update gravity timing
    void updateGravityTime()
    {
        lastGravityUpdate_ = std::chrono::steady_clock::now();
    }

    bool hasGravityIntervalElapsed()
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastGravityUpdate_).count();

        if (elapsedMs >= gravityTimeMs_)
        {
            lastGravityUpdate_ = now;
            return true;
        }
        return false;
    }

    // Scoring
    void addScore(int points)
    {
        score_ += points;
    }

    // Lines cleared
    void addClearedLines(int lines)
    {
        linesClearedThisLevel_ += lines;
    }

    // Attempt to level up
    bool tryLevelUp(int requiredLinesForLevelUp = 10)
    {
        if (linesClearedThisLevel_ >= requiredLinesForLevelUp)
        {
            ++level_;
            linesClearedThisLevel_ = 0;

            // Decrease gravity time, respecting the minimum limit
            gravityTimeMs_ -= gravityDecrementPerLevel_;
            if (gravityTimeMs_ < kMinGravityTimeMs)
            {
                gravityTimeMs_ = kMinGravityTimeMs;
            }
            return true;
        }
        return false;
    }

    // Getters
    int getScore() const { return score_; }
    int getLevel() const { return level_; }
    int getTotalLinesCleared() const { return totalLinesCleared; }
    int getLinesClearedThisLevel() const { return linesClearedThisLevel_; }
    int getGravityTimeMs() const { return gravityTimeMs_; }
};
