#pragma once

#include "nonSymmetricTetromino.hpp"
#include <memory>    // for std::unique_ptr
#include <stdexcept> // for std::invalid_argument

/*
    Class for Tetromino I (Straight piece)
*/
class TetrominoI : public Tetromino
{
public:
    TetrominoI(int x0, int y0, CellColorType color) : Tetromino(x0, y0, color)
    {
        shape = {{1, 1, 1, 1}}; // 4-block straight line
    }
};

/*
    Tetromino O (Square piece)
*/
class TetrominoO : public Tetromino
{
public:
    TetrominoO(int x0, int y0, CellColorType color) : Tetromino(x0, y0, color)
    {
        shape = {{1, 1},
                 {1, 1}}; // 2x2 square
    }
};

/*
    Class for Tetromino T (T-pose piece)
*/
class TetrominoT : public Tetromino
{
public:
    TetrominoT(int x0, int y0, CellColorType color) : Tetromino(x0, y0, color)
    {
        shape = {{1, 1, 1}, 
                 {0, 1, 0}}; // T
    }
};

/*
    Tetromino L (Orange L-shaped piece)
*/
class TetrominoL : public NonSymmetricTetromino
{
public:
    TetrominoL(int x0, int y0, CellColorType color, bool inverted = false) : NonSymmetricTetromino(x0, y0, color, inverted)
    {
        shape = {{1, 0},
                 {1, 0},
                 {1, 1}};

        if (inverted)
            this->invertShape();
    }
};

/*
    Tetromino S (Green S-shaped piece)
*/
class TetrominoS : public NonSymmetricTetromino
{
public:
    TetrominoS(int x0, int y0, CellColorType color, bool inverted = false) : NonSymmetricTetromino(x0, y0, color, inverted)
    {
        shape = {{0, 1, 1},
                 {1, 1, 0}};

        if (inverted)
            this->invertShape();
    }
};