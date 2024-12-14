#pragma once
#include "PlayerData.hpp"
#include <iostream>

class Player
{
public:
    // Construct a player with a given ID and name.
    // The server sets initial states such as score = 0 and isConnected = true when they join.
    Player(int id, const std::string &name)
    {
        data.playerID = id;
        data.playerName = name;
        data.score = 0;
        data.isConnected = true;
    }

    ~Player()
    {
        std::cout << "Player " << data.playerID << " was deleted " << std::endl;
    }

    // Access the player's read-only data.
    const PlayerData getData() const
    {
        return data;
    }

    // Modify player attributes. These methods are only run on the server,
    // ensuring the server remains the authority.
    void addScore(int amount)
    {
        data.score += amount;
        // The server can then notify all clients about the updated score
    }

    void setName(const std::string &newName)
    {
        data.playerName = newName;
        // Possibly broadcast name change to clients
    }

    void disconnect()
    {
        data.isConnected = false;
        // Handle cleanup, notify clients, etc.
    }

    bool isConnected() const
    {
        return data.isConnected;
    }

    int getPlayerID()
    {
        return data.playerID;
    }

private:
    PlayerData data;
};
