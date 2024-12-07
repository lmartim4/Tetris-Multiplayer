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
    static const std::vector<CellColorType> tetromino_colors;
    static const std::vector<char> types;
    bool gravity = false;
    // Default constructor & destructor
    Tetromino(int x0, int y0, CellColorType color) : x(x0), y(y0), color(color) {}
    virtual ~Tetromino() = default;

    int getX() const;
    int getY() const;

    // Rendering
    CellColorType getColor() const;

    bool getGravity() const;

    const std::vector<std::vector<int>> &getShape() const;

    void evolveStates(bool forward, TetrisAction lastMove);

    // Fall (gravity)
    void dropGravity();
};