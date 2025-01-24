#include "TetrominoFactory.hpp"
#include <cstdlib>
#include <stdexcept>

const std::vector<char> TetrominoFactory::tetrominoTypes = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};

const std::unordered_map<char, TetrominoFactory::TetrominoCreator> TetrominoFactory::tetrominoCreators = {
    {'I', [](Coordinate coord, CellColor color) { return std::make_unique<TetrominoI>(coord, color); }},
    {'O', [](Coordinate coord, CellColor color) { return std::make_unique<TetrominoO>(coord, color); }},
    {'T', [](Coordinate coord, CellColor color) { return std::make_unique<TetrominoT>(coord, color); }},
    {'L', [](Coordinate coord, CellColor color) { return std::make_unique<TetrominoL>(coord, color); }},
    {'J', [](Coordinate coord, CellColor color) { return std::make_unique<TetrominoL>(coord, color, true); }},
    {'S', [](Coordinate coord, CellColor color) { return std::make_unique<TetrominoS>(coord, color); }},
    {'Z', [](Coordinate coord, CellColor color) { return std::make_unique<TetrominoS>(coord, color, true); }},
};

std::unique_ptr<Tetromino> TetrominoFactory::createTetromino()
{
    char randomType = tetrominoTypes.at(rand() % tetrominoTypes.size());
    CellColor randomColor = Tetromino::tetromino_colors.at(rand() % Tetromino::tetromino_colors.size());

    int y0 = rand();
    auto it = tetrominoCreators.find(randomType);
    if (it != tetrominoCreators.end())
    {
        return it->second(Coordinate(0, y0), randomColor); // Call the creator function
    }
    throw std::invalid_argument("Invalid Tetromino type");
}
