#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Screen.hpp"
#include "TetrisCell.hpp"

constexpr int CELL_SIZE = 40;

class BoardScreen : public Screen
{
private:
    int BOARD_WIDTH = 10;
    int BOARD_HEIGHT = 10;
    ClientManager &clientManager;

    std::vector<std::vector<std::shared_ptr<TetrisCell>>> grid;

    void setupCells()
    {
        for (int y = 0; y < BOARD_HEIGHT; ++y)
        {
            std::vector<std::shared_ptr<TetrisCell>> row;
            for (int x = 0; x < BOARD_WIDTH; ++x)
            {
                auto cell = std::make_shared<TetrisCell>(
                    sf::Vector2f(CELL_SIZE, CELL_SIZE),
                    sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE),
                    sf::Color::Red);

                row.push_back(cell);
            }
            grid.push_back(row);
        }
    }

public:
    BoardScreen(ClientManager &clientManager) : clientManager(clientManager)
    {
    }

    void handleEvent(sf::Event event, ScreenManager &manager) override
    {
        handleKeyPress(event);
        
        for (auto &row : grid)
            for (auto &cell : row)
                cell->handleEvent(event);
    }

    void render(sf::RenderWindow &window) override
    {
        for (auto &row : grid)
            for (auto &cell : row)
                cell->render(window);
    }

    void updateBoardFromJson(const nlohmann::json &boardData)
    {
        std::cout << "Calling update tetris cells" << std::endl;

        int newWidth = boardData.at("width").get<int>();
        int newHeight = boardData.at("height").get<int>();

        if ((int)grid.size() != newHeight || (int)grid.at(0).size() != newWidth)
        {
            BOARD_WIDTH = newWidth;
            BOARD_HEIGHT = newHeight;
            grid.clear();
            setupCells();
        }

        auto cells = boardData.at("cells");
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            for (int x = 0; x < BOARD_WIDTH; x++)
            {
                sf::Color cellColor = sf::Color::White;

                if (cells[y][x].contains("c"))
                {
                    cellColor = TetrisCell::getColorFromType(cells[y][x]["c"]);
                    grid.at(y).at(x)->setColor(cellColor);
                }
            }
        }
    }

    void update(float deltaTime) override
    {
        if (clientManager.boardDataAvailable())
        {
            nlohmann::json newBoard = clientManager.getLastBoardState();

            std::cout << newBoard << std::endl;

            updateBoardFromJson(newBoard);
        }
    }

    void handleKeyPress(sf::Event event){
        if(event.type == sf::Event::KeyPressed)
            clientManager.onPressKey(event.key);
    }
};
