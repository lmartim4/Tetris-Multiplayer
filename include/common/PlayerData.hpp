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

    nlohmann::json serialize() const override;

    void deserialize(const nlohmann::json &data) override;
};
