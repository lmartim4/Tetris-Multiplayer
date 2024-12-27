#include "PlayerList.hpp"

nlohmann::json PlayerList::serialize() const
{
    nlohmann::json jsonData;
    nlohmann::json playersJson = nlohmann::json::array();
    for (const PlayerData &player : players)
        playersJson.push_back(player.serialize());
    jsonData["players"] = playersJson;

    return jsonData;
}

void PlayerList::deserialize(const nlohmann::json &data)
{
    if (data.contains("players"))
    {
        players.clear();
        for (const auto &playerData : data["players"])
        {
            PlayerData player;
            player.deserialize(playerData);
            players.push_back(player);
        }
    }
}
