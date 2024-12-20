#pragma once
#include <chrono>

class LevelData
{
private:
    int score;
    int level;
    int nLinesClearedThisLevel;

    const int minTimeMs = 400; // Minimum gravity time
    int gravityTimeMs;         // Current gravity time in ms
    int levelUpGravityTimeMs;  // Amount by which gravity time is reduced on level up
    std::chrono::steady_clock::time_point lastGravityTick;

public:
    // Constructor allows initialization of the level and scoring system
    LevelData(int initialLevel = 1,
              int initialScore = 0,
              int initialLinesCleared = 0,
              int initialGravityTimeMs = 500,
              int gravityReductionPerLevel = 50)
        : score(initialScore),
          level(initialLevel),
          nLinesClearedThisLevel(initialLinesCleared),
          gravityTimeMs(initialGravityTimeMs),
          levelUpGravityTimeMs(gravityReductionPerLevel),
          lastGravityTick(std::chrono::steady_clock::now())
    {
    }

    // Update the last gravity tick time to now
    void updateGravityTick()
    {
        lastGravityTick = std::chrono::steady_clock::now();
    }

    bool gravityIntervalPassed()
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastGravityTick).count();
        if (elapsed >= gravityTimeMs)
        {
            lastGravityTick = now;
            return true;
        }
        return false;
    }

    void addScore(int points)
    {
        score += points;
    }

    void addClearedLines(int lines)
    {
        nLinesClearedThisLevel += lines;
    }

    bool attemptLevelUp(int linesPerLevel = 10)
    {
        // If enough lines cleared, increment level
        if (nLinesClearedThisLevel >= linesPerLevel)
        {
            level++;
            nLinesClearedThisLevel = 0; // reset line count for the new level

            // Adjust gravity speed
            gravityTimeMs -= levelUpGravityTimeMs;
            if (gravityTimeMs < minTimeMs)
                gravityTimeMs = minTimeMs;
            return true;
        }
        
        return false;
    }

    // Getters
    int getScore() const { return score; }
    int getLevel() const { return level; }
    int getClearedLinesThisLevel() const { return nLinesClearedThisLevel; }
    int getGravityTimeMs() const { return gravityTimeMs; }
};
