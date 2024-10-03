#include "include/tetrominoFactory.hpp"
#include <iostream>

int main() {
    // Create a Tetromino I (straight piece)
    auto tetroI = TetrominoFactory::createTetromino('I', "cyan");

    // Create a Tetromino J (which is an inverted L)
    auto tetroJ = TetrominoFactory::createTetromino('J', "blue");

    // Create a Tetromino Z (which is an inverted S)
    auto tetroZ = TetrominoFactory::createTetromino('Z', "red");

    std::cout << tetroI->getX() << ", "  << tetroJ->getX() << ", " << tetroZ->getX() << std::endl;
    std::cout << tetroI->getY() << ", "  << tetroJ->getY() << ", " << tetroZ->getY() << std::endl;
    // std::cout << tetroI->getShape() << ", "  << tetroJ->getShape() << ", " << tetroZ->getShape() << std::endl;

    return 0;
}