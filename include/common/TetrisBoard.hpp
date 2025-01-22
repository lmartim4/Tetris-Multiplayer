#pragma once

#include <memory>
#include <vector>
#include "Cell.hpp"
#include "ISerializable.hpp"
#include "Logger.hpp"
#include "json.hpp"

/**
 * TetrisBoard: purely a container for cells,
 * no direct methods for moving Tetrominoes.
 */
class TetrisBoard : public ISerializable
{
private:
    Logger *logger;
    int width;
    int height;

    // The grid of cells
    std::vector<std::vector<std::shared_ptr<Cell>>> grid;
    void setupCells();
public:
    TetrisBoard(int h, int w);
    ~TetrisBoard();

    void printDebug() const;

    // Basic getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getNormalizedY(int y) const;

    // Access the grid
    std::vector<std::vector<std::shared_ptr<Cell>>> &getGrid() { return grid; }
    const std::vector<std::vector<std::shared_ptr<Cell>>> &getGrid() const { return grid; }

    // Board-level utility
    bool reachedTop() const;
    void clearBoard() const;

    // Serialization
    nlohmann::json serialize() const override;
    void deserialize(const nlohmann::json &data) override;
};
