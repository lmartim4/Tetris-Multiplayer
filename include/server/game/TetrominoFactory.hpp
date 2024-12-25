#pragma once

#include "TetrisBoard.hpp"
#include "Tetrominos.hpp"

class TetrominoFactory
{
public:
    static std::unique_ptr<Tetromino> createTetromino()
    {
        char randomType = Tetromino::types.at(rand() % Tetromino::types.size()); // Randomly select one tetromino
        CellColor randomColor = Tetromino::tetromino_colors.at(rand() % Tetromino::tetromino_colors.size());

        int y0 = rand();
    
        // No need to break as it returns directly
        switch (randomType)
        {
        /* Symmetric tetros */
        case 'I':
            return std::make_unique<TetrominoI>(Coordinate(0, y0), randomColor);
        case 'O':
            return std::make_unique<TetrominoO>(Coordinate(0, y0), randomColor);
        case 'T':
            return std::make_unique<TetrominoT>(Coordinate(0, y0), randomColor);

        /* Asymmetric tetros */
        /* L - J */
        case 'L':
            return std::make_unique<TetrominoL>(Coordinate(0, y0), randomColor);
        case 'J':
            // Create J from L inverted
            return std::make_unique<TetrominoL>(Coordinate(0, y0), randomColor, true);

        /* S - Z */
        case 'S':
            return std::make_unique<TetrominoS>(Coordinate(0, y0), randomColor);
        case 'Z':
            // Create Z from S inverted
            return std::make_unique<TetrominoS>(Coordinate(0, y0), randomColor, true);

        default:
            throw std::invalid_argument("Invalid Tetromino type");
        }
    }
};