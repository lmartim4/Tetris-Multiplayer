#pragma once

#include "Tetromino.hpp"
#include <memory>    // for std::unique_ptr
#include <stdexcept> // for std::invalid_argument

/*
    Class for Tetromino I (Straight piece)
*/
class TetrominoI : public Tetromino
{
public:
    TetrominoI(Coordinate coord, CellColor color) : Tetromino(coord, color)
    {
        shape.setShape({{1, 1, 1, 1}}); // 4-block straight line
    }
};

/*
    Tetromino O (Square piece)
*/
class TetrominoO : public Tetromino
{
public:
    TetrominoO(Coordinate coord, CellColor color) : Tetromino(coord, color)
    {
        shape.setShape({{1, 1},
                        {1, 1}}); // 2x2 square
    }
};

/*
    Class for Tetromino T (T-pose piece)
*/
class TetrominoT : public Tetromino
{
public:
    TetrominoT(Coordinate coord, CellColor color) : Tetromino(coord, color)
    {
        shape.setShape({{1, 1, 1},
                        {0, 1, 0}}); // T
    }
};

/*
    Tetromino L (Orange L-shaped piece)
*/
class TetrominoL : public NonSymmetricTetromino
{
public:
    TetrominoL(Coordinate coord, CellColor color, bool inverted = false) : NonSymmetricTetromino(coord, color, inverted)
    {
        shape.setShape({{1, 0}, {1, 0}, {1, 1}});

        if (inverted)
            this->shape.invertShape();
    }
};

/*
    Tetromino S (Green S-shaped piece)
*/
class TetrominoS : public NonSymmetricTetromino
{
public:
    TetrominoS(Coordinate coord, CellColor color, bool inverted = false) : NonSymmetricTetromino(coord, color, inverted)
    {
        shape.setShape({{0, 1, 1},
                        {1, 1, 0}});

        if (inverted)
            this->shape.invertShape();
    }
};