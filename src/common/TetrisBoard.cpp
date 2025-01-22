#include "TetrisBoard.hpp"
#include "Clock.hpp"

void TetrisBoard::setupCells()
{
    grid.clear();

    for (int x = 0; x < height; ++x)
    {
        std::vector<std::shared_ptr<Cell>> row;

        for (int y = 0; y < width; ++y)
        {
            auto cell = std::make_shared<Cell>(Coordinate(x, y));
            row.push_back(cell);
        }

        grid.push_back(row);
    }
}

TetrisBoard::TetrisBoard(int h, int w) : height(h), width(w)
{
    logger = new Logger("Tetris Board");
    setupCells();
}

void TetrisBoard::printDebug() const
{
    // system("clear");

    std::cout << "\nEstado da Board:\n"
              << std::endl;
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
            std::cout << (grid[x][y]->getState() == FALLING ? " # " : (grid[x][y]->getState() == EMPTY) ? " * "
                                                                                                        : " $ ");
        std::cout << std::endl;
    }

    std::cout << std::string(40, '-') << std::endl;

    std::cout << "\n\n\n"
              << std::endl;
}

TetrisBoard::~TetrisBoard()
{
    logger->console_log("Deleting Tetris-Board");
}

nlohmann::json TetrisBoard::serialize() const
{
    nlohmann::json boardJson;
    boardJson["width"] = width;
    boardJson["height"] = height;

    nlohmann::json cells = nlohmann::json::array();

    for (int x = 0; x < height; ++x)
    {
        nlohmann::json rowJson = nlohmann::json::array();

        for (int y = 0; y < width; ++y)
        {
            const auto &cell = grid.at(x).at(y);
            CellColor color = cell->getColor();
            CellState state = cell->getState(); // e.g. EMPTY, FALLING, FALLEN

            nlohmann::json cellObj;
            cellObj["c"] = color; // store color
            cellObj["s"] = state; // store state
            rowJson.push_back(cellObj);
        }
        cells.push_back(rowJson);
    }

    boardJson["cells"] = cells;
    return boardJson;
}

void TetrisBoard::deserialize(const nlohmann::json &data)
{
    if (!data.is_object())
        throw std::runtime_error("TetrisBoard::deserialize: JSON must be an object.");

    bool sizeChanged = false;
    if (data.contains("width") && data["width"].is_number())
    {
        int newWidth = data["width"];
        if (newWidth != width)
        {
            width = newWidth;
            sizeChanged = true;
        }
    }
    if (data.contains("height") && data["height"].is_number())
    {
        int newHeight = data["height"];
        if (newHeight != height)
        {
            height = newHeight;
            sizeChanged = true;
        }
    }

    // If size changed, rebuild the grid
    if (sizeChanged)
    {
        setupCells();
    }

    // Now parse the "cells" array
    if (!data.contains("cells") || !data["cells"].is_array())
    {
        logger->console_log("Could not update the board! Missing 'cells' array.");
        return;
    }

    const auto &cellsArray = data["cells"];
    for (size_t row = 0; row < cellsArray.size() && row < (size_t)height; ++row)
    {
        const auto &rowJson = cellsArray[row];
        if (!rowJson.is_array())
        {
            // skip or handle error
            continue;
        }

        for (size_t col = 0; col < rowJson.size() && col < (size_t)width; ++col)
        {
            const auto &cellObj = rowJson[col];
            // We expect two keys: "c" (color) and "s" (state)
            if (cellObj.is_object() && cellObj.contains("c") && cellObj.contains("s"))
            {
                CellColor colorVal = cellObj["c"];
                CellState stateVal = cellObj["s"];

                grid[row][col]->setColor(colorVal);
                grid[row][col]->setState(stateVal);
            }
        }
    }
}

bool TetrisBoard::reachedTop() const
{
    for (int y = 0; y < width; y++)
        if (grid.at(0).at(y)->getState() == FALLEN)
            return true;

    return false;
}

void TetrisBoard::clearBoard() const
{
    for (int x = 0; x < height; x++)
        for (int y = 0; y < width; y++)
            grid[x][y]->setEmpty();
}

int TetrisBoard::getNormalizedY(int y) const
{
    y %= width;

    if (y < 0)
        y += width;

    return y;
}