#include "EndGameData.hpp"

// Serialize the EndGameData object into a JSON object
nlohmann::json EndGameData::serialize() const
{
    nlohmann::json jsonData;

    jsonData["totalPoints"] = totalPoints;
    jsonData["gameTime"] = gameTime;
    jsonData["linesRemoved"] = linesRemoved;
    jsonData["finalLevel"] = finalLevel;

    // Serialize the players vector
    for (const auto &player : players)
    {
        jsonData["players"].push_back(player.serialize());
    }

    return jsonData;
}

// Deserialize a JSON object into the EndGameData object
void EndGameData::deserialize(const nlohmann::json &data)
{
    if (data.contains("totalPoints"))
        totalPoints = data["totalPoints"].get<int>();

    if (data.contains("gameTime"))
        gameTime = data["gameTime"].get<int>();

    if (data.contains("linesRemoved"))
        linesRemoved = data["linesRemoved"].get<int>();

    if (data.contains("finalLevel"))
        finalLevel = data["finalLevel"].get<int>();

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
