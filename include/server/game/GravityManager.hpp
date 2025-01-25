#pragma once

#include <chrono>

class GravityManager
{
private:
    static constexpr int kMinGravityTimeMs = 400;      // Minimum gravity time (hard cap)
    static constexpr int kInitialGravityTimeMs = 1000; // Starting gravity time
    static constexpr double kDifficultyRate = 0.2;     // Rate at which difficulty increases (adjustable)
    
    int gravityTimeMs = kInitialGravityTimeMs; // Current gravity time in milliseconds
    int level = 0;                             // Tracks the current level of difficulty

    std::chrono::steady_clock::time_point lastGravityUpdate = std::chrono::steady_clock::now();

public:
    GravityManager() = default;

    void increaseGravityDifficulty()
    {
        level++;

        // Calculate the new gravity time using an exponential decay function
        gravityTimeMs = static_cast<int>(
            kInitialGravityTimeMs * std::exp(-kDifficultyRate * level));

        if (gravityTimeMs < kMinGravityTimeMs)
        {
            gravityTimeMs = kMinGravityTimeMs;
        }
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
    int getGravityTimeMs() const { return gravityTimeMs; }
};
