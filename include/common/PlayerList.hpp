#pragma once

#include <vector>
#include "ISerializable.hpp"
#include "Player.hpp"
#include "json.hpp"

class PlayerList : public ISerializable
{
private:
    std::vector<PlayerData> players;

public:
    PlayerList() = default;
    ~PlayerList() override = default;

    // Serialization and deserialization
    nlohmann::json serialize() const override;
    void deserialize(const nlohmann::json &data) override;

    // Utility methods
    const std::vector<PlayerData> &getPlayers() const { return players; }
    void clearPlayers() { players.clear(); }
    void addPlayer(const PlayerData &player) { players.push_back(player); }
};