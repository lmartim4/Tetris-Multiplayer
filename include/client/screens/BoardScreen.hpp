#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Screen.hpp"
#include "TetrisCell.hpp"

constexpr int BOARD_WIDTH = 10;
constexpr int BOARD_HEIGHT = 10;
constexpr int CELL_SIZE = 40;

class BoardScreen : public Screen
{
    std::vector<std::vector<std::shared_ptr<TetrisCell>>> grid;

public:
    BoardScreen()
    {
        for (int y = 0; y < BOARD_HEIGHT; ++y)
        {
            std::vector<std::shared_ptr<TetrisCell>> row;
            for (int x = 0; x < BOARD_WIDTH; ++x)
            {
                auto cell = std::make_shared<TetrisCell>(
                    sf::Vector2f(CELL_SIZE, CELL_SIZE),
                    sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE),
                    sf::Color::Red // Default color
                );

                row.push_back(cell);
            }
            grid.push_back(row);
        }
    }

    void handleEvent(sf::Event event, ScreenManager &manager) override
    {
        for (auto &row : grid)
            for (auto &cell : row)
                cell->handleEvent(event);
    }

    void update(float deltaTime) override {}

    void render(sf::RenderWindow &window) override
    {
        for (auto &row : grid)
            for (auto &cell : row)
                cell->render(window);
    }
};
