#pragma once

#include <string>
#include <vector>
#include "CellColorType.hpp"
#include "TetrisAction.hpp"

struct Coordinate
{
    int x;
    int y;
};

class Tetromino
{
private:
    Coordinate coordinate;

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

    void evolveStates(bool forward, TetrisAction lastMove, bool gravity);
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