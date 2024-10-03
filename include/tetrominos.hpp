#ifndef TETROMINOS
#define TETROMINOS

// Nao precisa incluir a baseTetromino.hpp pq já deriva disso, talvez seja
// melhor separar cada uma na sua depois? Só pra n ficar nesse migué 
#include "nonSymmetricTetromino.hpp" 
#include <memory> // for std::unique_ptr
#include <stdexcept> // for std::invalid_argument

/* 
    Class for Tetromino I (Straight piece)
*/
class TetrominoI : public Tetromino
{
public:
    TetrominoI(char type, std::string color) : Tetromino(type, color)
    {
        shape = {{1, 1, 1, 1}};  // 4-block straight line
    }

    void rotateLeft() override
    {
        // Logic for rotating left for I piece (special 90-degree rotation handling)
    }

    void rotateRight() override
    {
        // Logic for rotating right for I piece (special 90-degree rotation handling)
    }

    void moveLeft() override { x--; }
    void moveRight() override { x++; }
    void dropSlow() override { y++; }
    void dropFast() override { y += 2; }
};

/*
    Tetromino O (Square piece)
*/
class TetrominoO : public Tetromino
{
public:
    TetrominoO(char type, std::string color) : Tetromino(type, color)
    {
        shape = {{1, 1},
                 {1, 1}}; // 2x2 square
    }

    // TODO 
    void rotateLeft() override
    {
        // No rotation for the O piece
    }

    // TODO 
    void rotateRight() override
    {
        // No rotation for the O piece
    }

    // TODO 
    void moveLeft() override { x--; }
    void moveRight() override { x++; }
    void dropSlow() override { y++; }
    void dropFast() override { y += 2; }
};

/*
    Tetromino L (Orange L-shaped piece)
*/
class TetrominoL : public NonSymmetricTetromino
{
public:
    TetrominoL(char type, std::string color, bool inverted = false) : NonSymmetricTetromino(type, color, inverted)
    {
        shape = {{1, 0},
                 {1, 0},
                 {1, 1}};
        
        // Needs to be called here to only invert it after the shape is defined
        // Possibly not very intuitive  
        if(inverted)
            this->invertShape();
    }
};

/*
    Tetromino S (Green S-shaped piece)
*/
class TetrominoS : public NonSymmetricTetromino
{
public:
    TetrominoS(char type, std::string color, bool inverted = false) : NonSymmetricTetromino(type, color, inverted)
    {
        shape = {{0, 1, 1},
                 {1, 1, 0}};

        // Needs to be called here to only invert it after the shape is defined
        // Possibly not very intuitive  
        if(inverted)
            this->invertShape();
    }
};


#endif