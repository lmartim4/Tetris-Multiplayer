#pragma once

#include <chrono>

class GravityManager
{
private:
    static constexpr int kMinGravityTimeMs = 400; // Minimum gravity time
    int gravityTimeMs = 500;                     // Current gravity time
    int gravityDecrementPerLevel = 50;           // How much to reduce per level
    std::chrono::steady_clock::time_point lastGravityUpdate = std::chrono::steady_clock::now();

public:
    GravityManager() = default;

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
