#pragma once

#include "TetrisBoard.hpp"
#include "Tetrominos.hpp"
#include <unordered_map>
#include <functional>
#include <memory>
#include <vector>

class TetrominoFactory
{
public:
    using TetrominoCreator = std::function<std::unique_ptr<Tetromino>(Coordinate, CellColor)>;

    static std::unique_ptr<Tetromino> createTetromino();

private:
    static const std::unordered_map<char, TetrominoCreator> tetrominoCreators;
    static const std::vector<char> tetrominoTypes; // List of available Tetromino types
};
