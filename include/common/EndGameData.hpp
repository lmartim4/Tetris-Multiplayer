#pragma once

#include <iostream>
#include <string>
#include "PlayerData.hpp"
#include "ISerializable.hpp"
#include "json.hpp"

struct EndGameData : public ISerializable
{
    int totalPoints = 0;
    int gameTime = 0;
    int linesRemoved = 0;
    int finalLevel = 0;

    std::vector<PlayerData> players; 

    nlohmann::json serialize() const override;
    void deserialize(const nlohmann::json &data) override;
    
};
