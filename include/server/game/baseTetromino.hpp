#pragma once

#include <string>
#include <vector>
#include "CellColorType.hpp"

enum MOVES
{
    EMPTY,
    LEFT,
    RIGHT,
    DOWN_FASTER,
    ROT_LEFT,
    ROT_RIGHT
};

class Tetromino
{
protected:
    std::vector<std::vector<int>> shape;
    int x, y;
    CellColorType color;
    int lastMove = EMPTY;
    bool gravity = false;

public:
    static const std::vector<CellColorType> tetromino_colors;
    static const std::vector<char> types;

    // Default constructor & destructor
    Tetromino(int x0, int y0, CellColorType color) : x(x0), y(y0), color(color) {}
    virtual ~Tetromino() = default;

    int getX() const;

    int getY() const;

    // Rendering
    CellColorType getColor() const;

    int getLastMove() const;

    bool getGravity() const;

    const std::vector<std::vector<int>> &getShape() const;

    void evolveStates(bool forward);

    void updateStates();

    void setLastAction(MOVES action);

    // Fall (gravity)
    void dropGravity();
};