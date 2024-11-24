#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include "baseTetromino.hpp"

class Board
{
private:

    // In terms of the number of cells  
    const int WIDTH;
    const int HEIGHT;
    const int CELL_SIZE;

    std::vector<std::vector<int>> grid; // Matriz 2D para representar a grid

public:
    Board(int W, int H, int cell_size);

    void render(sf::RenderWindow &window) const;

    bool checkCollision(const Tetromino &currentTetromino);
    bool placeTetromino(const Tetromino &currentTetromino, int x, int y); 

};

#endif