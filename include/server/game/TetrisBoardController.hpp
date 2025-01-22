#pragma once

#include "TetrisBoard.hpp"
#include "Tetromino.hpp"
#include "TetrisAction.hpp"

enum CollisionType
{
    NONE,               // No collision occurred
    FALLEN_OR_BOUNDARY, // Collided with a boundary or a fallen block
    FALLING_OTHER,      // Collided with another falling Tetromino
    OUT_OF_BOUNDS,      // Attempted to place Tetromino outside the board
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
    void placeTetromino(const std::shared_ptr<Tetromino> currentTetromino, bool fallen);
    int clearFullLines();
};
