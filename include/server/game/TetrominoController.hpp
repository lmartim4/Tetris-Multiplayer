#pragma once

#include "Tetromino.hpp"

class TetrominoController
{
private:
    const std::shared_ptr<Tetromino> tetromino;

public:
    TetrominoController(std::shared_ptr<Tetromino> tetromino);

    

    CollisionType checkCollision(std::shared_ptr<Tetromino> currentTetromino, TetrisAction action) const;
    void setCellState(const std::shared_ptr<Tetromino> currentTetromino, CellState state);
    int findAndClearFullLines();
};