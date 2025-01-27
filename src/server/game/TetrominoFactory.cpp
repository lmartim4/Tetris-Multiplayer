#include "TetrominoFactory.hpp"
#include <random>
#include <stdexcept>
#include <iostream>

const std::vector<char> TetrominoFactory::tetrominoTypes = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};

const std::unordered_map<char, TetrominoFactory::TetrominoCreator> TetrominoFactory::tetrominoCreators = {
    {'I', [](Coordinate coord, CellColor color)
     {
         return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{1, 1, 1, 1}});
     }},
    {'O', [](Coordinate coord, CellColor color)
     {
         return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{1, 1}, {1, 1}});
     }},
    {'T', [](Coordinate coord, CellColor color)
     {
         return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{0, 1, 0}, {1, 1, 1}});
     }},
    {'L', [](Coordinate coord, CellColor color)
     {
         return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{1, 0}, {1, 0}, {1, 1}});
     }},
    {'J', [](Coordinate coord, CellColor color)
     {
         return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{0, 1}, {0, 1}, {1, 1}});
     }},
    {'S', [](Coordinate coord, CellColor color)
     {
         return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{0, 1, 1}, {1, 1, 0}});
     }},
    {'Z', [](Coordinate coord, CellColor color)
     {
         return std::make_unique<Tetromino>(coord, color, std::vector<std::vector<int>>{{1, 1, 0}, {0, 1, 1}});
     }},
};

std::unique_ptr<Tetromino> TetrominoFactory::createTetromino()
{
    // Random device and generator
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distType(0, tetrominoTypes.size() - 1);
    static std::uniform_int_distribution<> distColor(0, Tetromino::tetromino_colors.size() - 1);
    static std::uniform_int_distribution<> distY(0, 1023);

    // Generate random type, color, and coordinate
    char randomType = tetrominoTypes.at(distType(gen));
    CellColor randomColor = Tetromino::tetromino_colors.at(distColor(gen));
    int y0 = distY(gen);

    auto it = tetrominoCreators.find(randomType);
    if (it != tetrominoCreators.end())
        return it->second(Coordinate(0, y0), randomColor);

    throw std::invalid_argument("Invalid Tetromino type: " + std::string(1, randomType));
}
