#include "TetrominoFactory.hpp"
#include <cstdlib>
#include <stdexcept>

const std::vector<char> TetrominoFactory::tetrominoTypes = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};

const std::unordered_map<char, TetrominoFactory::TetrominoCreator> TetrominoFactory::tetrominoCreators = {

    {'I', [](Coordinate coord, CellColor color) { return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{1, 1, 1, 1}});
                                                                                                                                 }},

    {'O', [](Coordinate coord, CellColor color) { return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{1, 1},
                                                                                                                                 {1, 1}});
                                                                                                                                 }},

    {'T', [](Coordinate coord, CellColor color) { return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{0, 1, 0},
                                                                                                                                 {1, 1, 1}});
                                                                                                                                 }},

    {'L', [](Coordinate coord, CellColor color) { return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{1, 0},
                                                                                                                                 {1, 0},
                                                                                                                                 {1, 1}});
                                                                                                                                 }},

    {'J', [](Coordinate coord, CellColor color) { return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{0, 1},
                                                                                                                                 {0, 1},
                                                                                                                                 {1, 1}});
                                                                                                                                 }},

    {'S', [](Coordinate coord, CellColor color) { return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{0, 1, 1},
                                                                                                                                 {1, 1, 0}}); 
                                                                                                                                }},

    {'Z', [](Coordinate coord, CellColor color) { return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{1, 1, 0},
                                                                                                                                 {0, 1, 1}});
                                                                                                                                }},
};

std::unique_ptr<Tetromino> TetrominoFactory::createTetromino()
{
    char randomType = tetrominoTypes.at(rand() % tetrominoTypes.size());
    CellColor randomColor = Tetromino::tetromino_colors.at(rand() % Tetromino::tetromino_colors.size());

    int y0 = rand();
    auto it = tetrominoCreators.find(randomType);
    if (it != tetrominoCreators.end())
    {
        return it->second(Coordinate(0, y0), randomColor);
    }
    throw std::invalid_argument("Invalid Tetromino type");
}
