#pragma once

// Needed for std::reverse
#include "algorithm"
#include "Tetromino.hpp"

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