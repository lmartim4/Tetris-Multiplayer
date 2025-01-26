#include "EndGameData.hpp"

// Serialize the EndGameData object into a JSON object
nlohmann::json EndGameData::serialize() const
{
    nlohmann::json players_json;

    for (const auto &player : players)
        players_json.push_back(player.serialize());

    return {
        {"totalPoints", totalPoints},
        {"gameTime", gameTime},
        {"linesRemoved", linesRemoved},
        {"players", players_json}};
}

void EndGameData::deserialize(const nlohmann::json &data)
{
    std::cout << data << std::endl;
    totalPoints = data.at("totalPoints").get<int>();
    gameTime = data.at("gameTime").get<int>();
    linesRemoved = data.at("linesRemoved").get<int>();

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
