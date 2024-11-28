#ifndef TETROMINO_FACTORY
#define TETROMINO_FACTORY

#include "tetrominos.hpp"

class TetrominoFactory
{
public:                                                                            
    static std::unique_ptr<Tetromino> createTetromino(char type, int x0, int y0, sf::Color color)
    {
        // No need to break as it returns directly
        switch (type)
        {
        /* Symmetric tetros */
        case 'I':
            return std::make_unique<TetrominoI>(type,x0,y0, color);
        case 'O':
            return std::make_unique<TetrominoO>(type,x0,y0, color);
        case 'T':
            return std::make_unique<TetrominoT>(type,x0,y0, color);

        /* Asymmetric tetros */
        /* L - J */
        case 'L':
            return std::make_unique<TetrominoL>(type,x0,y0, color);
        case 'J':
            // Create J from L inverted
            return std::make_unique<TetrominoL>(type,x0,y0, color, true);

        /* S - Z */
        case 'S':
            return std::make_unique<TetrominoS>(type,x0,y0, color);
        case 'Z':
            // Create Z from S inverted
            return std::make_unique<TetrominoS>(type,x0,y0, color, true);

        default:
            throw std::invalid_argument("Invalid Tetromino type");
        }
    }
};

#endif