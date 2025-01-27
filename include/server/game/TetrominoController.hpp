#pragma once

#include "Tetromino.hpp"

class TetrominoController
{
private:
    void rotate(std::shared_ptr<Tetromino> &tetromino, TetrisAction action);

public:
    TetrominoController() = default;
    ~TetrominoController() { std::cout << "Deleting TetrominoController\n"; };
    void setCanMove(std::shared_ptr<Tetromino> &tetromino, bool b);
    void setLockedInPlace(std::shared_ptr<Tetromino> &tetromino, bool b);
    bool canMove(const std::shared_ptr<Tetromino> &tetromino);
    bool isLockedInPlace(const std::shared_ptr<Tetromino> &tetromino);
    void transform(std::shared_ptr<Tetromino> &tetromino, TetrisAction action, bool forward);
};