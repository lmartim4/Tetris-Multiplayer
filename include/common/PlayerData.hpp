#pragma once

#include "ISerializable.hpp"

struct PlayerData : public ISerializable
{
    std::string playerName;
    int id = -1;
    int score = 0;
    bool isConnected = false;

    nlohmann::json serialize() const override;
    void deserialize(const nlohmann::json &data) override;
};
