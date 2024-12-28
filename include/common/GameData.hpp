#pragma once

#include "ISerializable.hpp"

class GameData : public ISerializable
{
private:
    int score = 0;
    int level = 0;
    int totalLinesCleared = 0;
    int linesClearedOnThisLevel = 0;

public:
    // Constructors / Destructor
    GameData() = default;
    ~GameData() override = default;

    // Attempt to level up
    bool tryLevelUp(int requiredLinesForLevelUp = 10)
    {
        if (linesClearedOnThisLevel >= requiredLinesForLevelUp)
        {
            level++;
            linesClearedOnThisLevel = 0;
            return true;
        }
        return false;
    }

    // ---- Getters ----
    int getScore() const { return score; }
    int getLevel() const { return level; }
    int getTotalLinesCleared() const { return totalLinesCleared; }
    int getLinesClearedOnThisLevel() const { return linesClearedOnThisLevel; }

    // ---- Setters ----
    void addScore(int s) { score += s; }
    void setLevel(int l) { level = l; }
    void addTotalLinesCleared(int t) { totalLinesCleared += t; }
    void addLinesClearedOnThisLevel(int c) { linesClearedOnThisLevel += c; }

    // ---- ISerializable interface ----
    nlohmann::json serialize() const override
    {
        nlohmann::json j;
        j["score"] = score;
        j["level"] = level;
        j["totalLinesCleared"] = totalLinesCleared;
        j["linesClearedOnThisLevel"] = linesClearedOnThisLevel;
        return j;
    }

    void deserialize(const nlohmann::json &data) override
    {
        // Use 'contains' to safely check for JSON fields
        if (data.contains("score"))
            data["score"].get_to(score);
        if (data.contains("level"))
            data["level"].get_to(level);
        if (data.contains("totalLinesCleared"))
            data["totalLinesCleared"].get_to(totalLinesCleared);
        if (data.contains("linesClearedOnThisLevel"))
            data["linesClearedOnThisLevel"].get_to(linesClearedOnThisLevel);
    }
};
