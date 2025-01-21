#pragma once

#include "TetrisBoard.hpp"
#include "Tetromino.hpp"
#include "TetrisAction.hpp"

class TetrisBoardController
{
private:
    const TetrisBoard &board;

public:
    TetrisBoardController(TetrisBoard &board);

    void clearFallingTetromino(const Tetromino &currentTetromino);
    void clearFallenTetrominos();

    bool checkCollision(Tetromino &currentTetromino, TetrisAction action) const;
    void placeTetromino(const Tetromino &currentTetromino, bool fallen);
    int clearFullLines();
};
