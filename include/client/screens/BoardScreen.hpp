#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Screen.hpp"
#include "TetrisCell.hpp"
#include "ClientManager.hpp"

class BoardScreen : public Screen
{
private:
    int BOARD_WIDTH = 10;
    int BOARD_HEIGHT = 10;
    int CELL_SIZE = 30;
    ClientManager &clientManager;
    
    nlohmann::json lastBoard;
    std::vector<std::vector<std::shared_ptr<TetrisCell>>> grid;

    void setupCells();

public:
    BoardScreen(ClientManager &clientManager);

    void handleEvent(sf::Event event, ScreenManager &manager) override;
    void render(sf::RenderWindow &window) override;
    void updateBoardFromJson(const nlohmann::json &boardData);
    void update(float deltaTime) override;
    void handleKeyPress(sf::Event event);
};
