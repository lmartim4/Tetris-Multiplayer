#pragma once

#include "TetrisBoard.hpp"
#include "Tetromino.hpp"
#include "TetrisAction.hpp"

enum CollisionType
{
    NONE,          // No collision occurred
    FALLING_OTHER, // Collided with another falling Tetromino
    FALLEN_FIXED,  // Collided with a boundary or a fallen block
    GROUND,        // Attempted to place Tetromino outside the board
};

class BoardController
{
private:
    std::shared_ptr<TetrisBoard> board;
    void printMatrix(const std::vector<std::vector<int>> &matrix, const std::string &label) const;

public:
    BoardController(std::shared_ptr<TetrisBoard> board);

    std::shared_ptr<TetrisBoard> getBoard();

    void clearFallingTetromino(const std::shared_ptr<Tetromino> currentTetromino);
    void clearFallenTetrominos();

    CollisionType checkCollision(std::shared_ptr<Tetromino> currentTetromino, TetrisAction action, std::shared_ptr<TetrominoController> tetrominoController) const;
    void setCellState(int ownerID, const std::shared_ptr<Tetromino> currentTetromino, CellState state);
    int findAndClearFullLines();
};
