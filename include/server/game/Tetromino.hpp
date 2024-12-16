#pragma once

#include <string>
#include <vector>
#include "CellColorType.hpp"
#include "TetrisAction.hpp"

class Tetromino
{
protected:
    std::vector<std::vector<int>> shape;
    int x, y;
    CellColorType color;

public:
    bool gravity = false;
    static const std::vector<CellColorType> tetromino_colors;
    static const std::vector<char> types;

    Tetromino(int x0, int y0, CellColorType color) : x(x0), y(y0), color(color) {}
    virtual ~Tetromino() = default;

    int getX() const;
    int getY() const;

    CellColorType getColor() const;

    bool getGravity() const;

    const std::vector<std::vector<int>> &getShape() const;

    void evolveStates(bool forward, TetrisAction lastMove);

    void dropGravity();
};