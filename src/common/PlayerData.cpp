#include "PlayerData.hpp"

nlohmann::json PlayerData::serialize() const
{
    return {
        {"id", id},
        {"playerName", playerName},
        {"score", score},
        {"isConnected", isConnected}};
}

void PlayerData::deserialize(const nlohmann::json &data)
{
    id = data.at("id").get<int>();
    playerName = data.at("playerName").get<std::string>();
    score = data.at("score").get<int>();
    isConnected = data.at("isConnected").get<bool>();
}