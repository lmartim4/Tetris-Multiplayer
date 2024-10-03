#ifndef TETROMINO_FACTORY
#define TETROMINO_FACTORY

#include "tetrominos.hpp"

class TetrominoFactory
{
public:                                                                            
    static std::unique_ptr<Tetromino> createTetromino(char type, std::string color)
    {
        // No need to break as it returns directly
        switch (type)
        {
        /* Symmetric tetros */
        case 'I':
            return std::make_unique<TetrominoI>(type, color);
        case 'O':
            return std::make_unique<TetrominoO>(type, color);

        /* Asymmetric tetros */
        /* L - J */
        case 'L':
            return std::make_unique<TetrominoL>(type, color);
        case 'J':
            // Create J from L inverted
            return std::make_unique<TetrominoL>(type, color, true);

        /* S - Z */
        case 'S':
            return std::make_unique<TetrominoS>(type, color);
        case 'Z':
            // Create Z from S inverted
            return std::make_unique<TetrominoS>(type, color, true);

        default:
            throw std::invalid_argument("Invalid Tetromino type");
        }
    }
};

#endif