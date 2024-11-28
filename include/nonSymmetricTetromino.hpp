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

public:
    // Basically just calls the default constructor & inverts the shape
    NonSymmetricTetromino(char type, int x0, int y0, sf::Color color, bool invert = false) : Tetromino(type, x0, y0, color)
    {
    }

};

#endif