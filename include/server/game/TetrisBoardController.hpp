#pragma once

#include "TetrisBoard.hpp"
#include "Tetromino.hpp"
#include "TetrisAction.hpp"

enum CollisionType
{
    NONE,          // No collision occurred
    FALLING_OTHER, // Collided with another falling Tetromino
    FALLEN_FIXED, // Collided with a boundary or a fallen block
    GROUND,       // Attempted to place Tetromino outside the board
};

class TetrisBoardController
{
private:
    const std::shared_ptr<TetrisBoard> board;

public:
    TetrisBoardController(std::shared_ptr<TetrisBoard> board);

    void clearFallingTetromino(const std::shared_ptr<Tetromino> currentTetromino);
    void clearFallenTetrominos();

    CollisionType checkCollision(std::shared_ptr<Tetromino> currentTetromino, TetrisAction action) const;
    void setCellState(const std::shared_ptr<Tetromino> currentTetromino, CellState state);
    int findAndClearFullLines();
};
