#ifndef TETROMINOS
#define TETROMINOS

#include "nonSymmetricTetromino.hpp"
#include <memory>    // for std::unique_ptr
#include <stdexcept> // for std::invalid_argument

/*
    Class for Tetromino I (Straight piece)
*/
class TetrominoI : public Tetromino
{
public:
    TetrominoI(char type, int x0, int y0, sf::Color color) : Tetromino(type, x0, y0, color)
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
    TetrominoO(char type, int x0, int y0, sf::Color color) : Tetromino(type, x0, y0, color)
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
    TetrominoT(char type, int x0, int y0, sf::Color color) : Tetromino(type, x0, y0, color)
    {
        shape = {{1, 1, 1}
                ,{0, 1, 0} }; // T 
    }
};

/*
    Tetromino L (Orange L-shaped piece)
*/
class TetrominoL : public NonSymmetricTetromino
{
public:
    TetrominoL(char type, int x0, int y0, sf::Color color, bool inverted = false) : NonSymmetricTetromino(type, x0, y0, color, inverted)
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
    TetrominoS(char type, int x0, int y0, sf::Color color, bool inverted = false) : NonSymmetricTetromino(type, x0, y0, color, inverted)
    {
        shape = {{0, 1, 1},
                 {1, 1, 0}};

        if (inverted)
            this->invertShape();
    }
};

#endif