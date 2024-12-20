#pragma once

#include <string>
#include <vector>
#include "CellColorType.hpp"
#include "TetrisAction.hpp"

struct Coordinate
{
    int x;
    int y;
    Coordinate(int xVal, int yVal) : x(xVal), y(yVal) {}
};

class Tetromino
{
private:
    Coordinate coordinate;
    bool hasChanged;

protected:
    std::vector<std::vector<int>> shape;
    CellColorType color;

public:
    static const std::vector<CellColorType> tetromino_colors;
    static const std::vector<char> types;

    Coordinate getCoordinate() const { return coordinate; }

    Tetromino(Coordinate coord, CellColorType color) : coordinate(coord), color(color) {}
    virtual ~Tetromino() = default;

    CellColorType getColor() const;

    const std::vector<std::vector<int>> &getShape() const;

    bool shouldBroadcastState()
    {
        if (hasChanged)
        {
            hasChanged = false;
            return true;
        }
        
        return false;
    }

    void evolveStates(bool forward, TetrisAction lastMove, bool gravity);

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
    NonSymmetricTetromino(Coordinate coord, CellColorType color, bool invert = false) : Tetromino(coord, color) {}
};