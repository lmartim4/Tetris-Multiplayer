#ifndef NON_SYMMETRIC_TETROMINO
#define NON_SYMMETRIC_TETROMINO

// Needed for std::reverse
#include "algorithm"
#include "baseTetromino.hpp"

class NonSymmetricTetromino : public Tetromino
{

protected:
    // Only called locally, in the constructor
    void invertShape()
    {
        // Logic to invert the shape based on the tetromino type
        for (auto &row : shape)
        {
            std::reverse(row.begin(), row.end());
        }
    }

    // Flag to determine if the shape is inverted
    bool inverted;

public:
    // Basically just calls the default constructor & inverts the shape
    NonSymmetricTetromino(char type, std::string color, bool invert = false) : Tetromino(type, color), inverted(invert)
    {
    }

    void rotateLeft() override
    {
        // Logic for rotating left (specific to non-symmetric shapes)
    }

    void rotateRight() override
    {
        // Logic for rotating right (specific to non-symmetric shapes)
    }

    void moveLeft() override { x--; }
    void moveRight() override { x++; }
    void dropSlow() override { y++; }

    // Nothing like this, the idea is to drop all the way to the bottom, but let's entertain
    // for a moment the possibility that it's just adjusting the speed of the block
    void dropFast() override { y += 2; }
};

#endif