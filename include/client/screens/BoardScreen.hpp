#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Screen.hpp"
#include "TetrisCell.hpp"

class BoardScreen : public Screen
{
private:
    int BOARD_WIDTH = 10;
    int BOARD_HEIGHT = 10;
    int CELL_SIZE = 30;
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
        // Check for required keys in the JSON
        if (!boardData.contains("width") || !boardData.contains("height") || !boardData.contains("cells"))
        {
            std::cerr << "Error: JSON is missing required keys (width, height, or cells)" << std::endl;
            return;
        }

        // Check that width and height are valid integers
        if (!boardData.at("width").is_number_integer() || !boardData.at("height").is_number_integer())
        {
            std::cerr << "Error: JSON width or height is not an integer" << std::endl;
            return;
        }

        // Extract width and height
        int newWidth = boardData.at("width").get<int>();
        int newHeight = boardData.at("height").get<int>();

        // Check that cells is a valid 2D array
        if (!boardData.at("cells").is_array() || boardData.at("cells").size() != newHeight)
        {
            std::cerr << "Error: JSON cells array does not match the specified height" << std::endl;
            return;
        }

        for (const auto &row : boardData.at("cells"))
        {
            if (!row.is_array() || row.size() != newWidth)
            {
                std::cerr << "Error: JSON cells array does not match the specified width" << std::endl;
                return;
            }
        }

        // Proceed with processing the data
        if ((int)grid.size() != newHeight || (int)grid.at(0).size() != newWidth)
        {
            BOARD_WIDTH = newWidth;
            BOARD_HEIGHT = newHeight;
            grid.clear();
            setupCells();
        }

        // Update cell colors
        auto cells = boardData.at("cells");
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            for (int x = 0; x < BOARD_WIDTH; x++)
            {
                sf::Color cellColor = sf::Color::White;

                // Check if cell contains "c" and process color
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
            updateBoardFromJson(newBoard);
        }
    }

    void handleKeyPress(sf::Event event)
    {
        if (event.type == sf::Event::KeyPressed)
            clientManager.onPressKey(event.key);
    }
};
