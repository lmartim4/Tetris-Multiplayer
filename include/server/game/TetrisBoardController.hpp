#pragma once

#include "TetrisBoard.hpp"
#include "Tetromino.hpp"
#include "TetrisAction.hpp"

class TetrisBoardController
{
private:
    const std::shared_ptr<TetrisBoard> board;

public:
    TetrisBoardController(std::shared_ptr<TetrisBoard> board);

    void clearFallingTetromino(const std::shared_ptr<Tetromino> currentTetromino);
    void clearFallenTetrominos();

    bool checkCollision(std::shared_ptr<Tetromino> currentTetromino, TetrisAction action) const;
    void placeTetromino(const std::shared_ptr<Tetromino> currentTetromino, bool fallen);
    int clearFullLines();
};
