#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include "baseTetromino.hpp"
#include <iostream>
#include "cell.hpp"

class Board
{
private:
    sf::RenderWindow window;

    std::vector<std::vector<Cell>> grid; // Matriz 2D para representar a grid e seus estados/logica

public:
    // In terms of the number of cells
    static const int WIDTH;
    static const int HEIGHT;
    static const int CELL_SIZE;

    Board();

    void printStatus();
    bool windowIsOpen();
    void handleInput(Tetromino &currentTetromino);
    void render();

    // Melhor seria se fosse const, mas é a vida, não consigo pensar em outro jeito de
    // checar a colisao sem mudar o objeto e ver se dá merda, na prática faz-se a ação inversa depois, então safe
    bool checkCollision(Tetromino &currentTetromino);
    bool placeTetromino(const Tetromino &currentTetromino, bool fallen);
    static int normalizedY(int y);
    void clearFallingTetrominos();
    std::vector<std::vector<Cell>> &getGrid();
};

#endif