#pragma once

#include <enet/enet.h>
#include <thread>
#include <map>

#include "NetworkManager.hpp"
#include "Player.hpp"

// In ServerManager.hpp or a suitable header
#include <json.hpp>
#include <random>

class ServerManager : public NetworkManager
{
private:
    int getNextAvailablePlayerID();
    void broadcastPlayerList();
    void createPlayerAndLink(ENetPeer *peer);

protected:
    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;

public:
    ServerManager(uint16_t port = 12345);
    ~ServerManager() {};

    void start_game();

    inline nlohmann::json createRandomBoard(int width, int height)
    {
        nlohmann::json boardJson;
        boardJson["width"] = width;
        boardJson["height"] = height;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> fillDist(0, 1); // Decide filled or not

        // For variety, we will store a small enum-like int for colorType
        // 0 = empty, 1 = red, 2 = green, etc.
        std::uniform_int_distribution<> colorDist(1, 4);

        nlohmann::json cells = nlohmann::json::array();
        for (int y = 0; y < height; ++y)
        {
            nlohmann::json row = nlohmann::json::array();
            for (int x = 0; x < width; ++x)
            {
                bool filled = (fillDist(gen) == 1);
                int colorType = filled ? colorDist(gen) : 0; // 0 = Empty
                nlohmann::json cellObj;
                cellObj["c"] = colorType;
                row.push_back(cellObj);
            }
            cells.push_back(row);
        }

        boardJson["cells"] = cells;
        return boardJson;
    }
};
