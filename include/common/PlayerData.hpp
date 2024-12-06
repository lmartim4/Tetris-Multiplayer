#pragma once
#include <string>
#include "ISerializable.hpp"
#include "json.hpp"

struct PlayerData : public ISerializable
{
    int playerID = -1;
    std::string playerName;
    int score = 0;
    bool isConnected = false;

    nlohmann::json serialize() const override
    {
        return {
            {"playerID", playerID},
            {"playerName", playerName},
            {"score", score},
            {"isConnected", isConnected}};
    }

    void deserialize(const nlohmann::json &data) override
    {
        playerID = data.at("playerID").get<int>();
        playerName = data.at("playerName").get<std::string>();
        score = data.at("score").get<int>();
        isConnected = data.at("isConnected").get<bool>();
    }
};
