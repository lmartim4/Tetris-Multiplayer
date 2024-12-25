#pragma once

#include <iostream>
#include "TetrisAction.hpp"
#include "ThreadSafeQueue.hpp"
#include "PlayerData.hpp"

class Player
{
public:
    Player(int id, const std::string &name)
    {
        data.playerID = id;
        data.playerName = name;
        data.score = 0;
    }

    ~Player()
    {
        std::cout << "Player " << data.playerID << " was deleted " << std::endl;
    }

    const PlayerData getData() const
    {
        return data;
    }

    void addScore(int amount)
    {
        data.score += amount;
    }

    void setName(const std::string &newName)
    {
        data.playerName = newName;
    }

    void disconnect()
    {
        data.isConnected = false;
    }

    bool isConnected() const
    {
        return data.isConnected;
    }

    int getPlayerID()
    {
        return data.playerID;
    }

    void enqueueAction(const TetrisAction &action)
    {
        actions.push(action);
    }

    bool popAction(TetrisAction &action)
    {
        return actions.pop(action);
    }

private:
    ThreadSafeQueue<TetrisAction> actions;
    PlayerData data;
};
