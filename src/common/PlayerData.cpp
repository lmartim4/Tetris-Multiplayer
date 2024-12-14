#include "PlayerData.hpp"

nlohmann::json PlayerData::serialize() const
{
    return {
        {"playerID", playerID},
        {"playerName", playerName},
        {"score", score},
        {"isConnected", isConnected}};
}

void PlayerData::deserialize(const nlohmann::json &data)
{
    playerID = data.at("playerID").get<int>();
    playerName = data.at("playerName").get<std::string>();
    score = data.at("score").get<int>();
    isConnected = data.at("isConnected").get<bool>();
}