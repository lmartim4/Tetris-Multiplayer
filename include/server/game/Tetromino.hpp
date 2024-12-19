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

class NonSymmetricTetromino : public Tetromino
{

protected:
    // Only called locally, in the constructor
    void invertShape()
    {
        // Logic to invert the shape based on the tetromino type
        for (auto &y : shape)
        {
            std::reverse(y.begin(), y.end());
        }
    }

public:
    // Basically just calls the default constructor & inverts the shape
    NonSymmetricTetromino(int x0, int y0, CellColorType color, bool invert = false)
        : Tetromino(x0, y0, color)
    {
    }
};