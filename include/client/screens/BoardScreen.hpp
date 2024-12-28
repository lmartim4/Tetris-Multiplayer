#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Screen.hpp"
#include "CellRenderer.hpp"
#include "TetrisBoard.hpp"
#include "ClientManager.hpp"

class BoardScreen : public Screen
{
private:
    const int CELL_SIZE = 30;

    TetrisBoard board;
    ClientManager &clientManager;
    std::vector<std::vector<std::shared_ptr<CellRenderer>>> renderGrid;

    sf::Text score;
    sf::Text lines;
    sf::Text level;

    void setupRenderers();

public:
    BoardScreen(ClientManager &clientManager);

    void handleEvent(sf::Event event, ScreenManager &manager) override;
    void render(sf::RenderWindow &window) override;
    void updateBoardFromJson(const nlohmann::json &boardData);
    void update(float deltaTime) override;
    void handleKeyPress(sf::Event event);
};
