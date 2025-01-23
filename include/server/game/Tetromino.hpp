#pragma once

#include <string>
#include <vector>
#include "CellColor.hpp"
#include "TetrisAction.hpp"
#include "Coordinate.hpp"

class Tetromino
{
private:
    Coordinate coordinate;

    int pieceId;
    bool hasChanged;

protected:
    std::vector<std::vector<int>> shape;
    CellColor color;

public:
    static const std::vector<CellColor> tetromino_colors;
    static const std::vector<char> types;

    Coordinate getCoordinate() const { return coordinate; }

    Tetromino(Coordinate coord, CellColor color) : coordinate(coord), color(color)
    {
        static int nextId = 1;
        pieceId = nextId++;
        
        hasChanged = true;
    }
    virtual ~Tetromino() = default;

    CellColor getColor() const;

    const std::vector<std::vector<int>> &getShape() const;

    int getId() const { return pieceId; }

    bool shouldBroadcastState()
    {
        if (hasChanged)
        {
            hasChanged = false;
            return true;
        }

        return false;
    }

    void evolveStates(bool forward, TetrisAction lastMove);

    void rotate(bool clockwise)
    {
        size_t rows = shape.size();
        size_t cols = shape[0].size();

        std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows));

        if (clockwise)
        {
            // Rotate right (clockwise)
            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < cols; ++j)
                {
                    rotated[j][rows - i - 1] = shape[i][j];
                }
            }
        }
        else
        {
            // Rotate left (counter-clockwise)
            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < cols; ++j)
                {
                    rotated[cols - j - 1][i] = shape[i][j];
                }
            }
        }

        shape = rotated;
    }
};

class NonSymmetricTetromino : public Tetromino
{
protected:
    void invertShape()
    {
        for (auto &y : shape)
            std::reverse(y.begin(), y.end());
    }

public:
    NonSymmetricTetromino(Coordinate coord, CellColor color, bool invert = false) : Tetromino(coord, color) {}
};