#pragma once

#include "TetrisBoard.hpp"
#include "Tetromino.hpp"
#include "TetrisAction.hpp"  // enum or similar

/**
 * TetrisBoardController: implements logic for manipulating
 * Tetrominoes on a TetrisBoard.
 */
class TetrisBoardController
{
private:
    TetrisBoard &board; // reference to the board container

public:
    // The controller needs a reference (or pointer) to an existing board
    TetrisBoardController(TetrisBoard &board);

    void clearFallingTetrominos();
    void clearFallenTetrominos();

    // Logic methods
    bool checkCollision(Tetromino &currentTetromino, TetrisAction action) const;
    void placeTetromino(const Tetromino &currentTetromino, bool fallen);
    int  clearFullLines();
    // etc.
};
