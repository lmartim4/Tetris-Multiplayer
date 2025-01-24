#pragma once

#include "Tetromino.hpp"
#include "TetrominoFactory.hpp"
#include "TetrisBoard.hpp"

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
    static const std::vector<char> tetrominoTypes;
};
