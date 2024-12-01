#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include "baseTetromino.hpp"
#include <iostream>
#include "cell.hpp"

class Board
{
private:

    // In terms of the number of cells  
    const int WIDTH;
    const int HEIGHT;
    const int CELL_SIZE;

    std::vector<std::vector<Cell>> grid; // Matriz 2D para representar a grid e seus estados/logica

public:
    Board(int W, int H, int cell_size);

    void render(sf::RenderWindow &window) ;

    // Melhor seria se fosse const, mas é a vida, não consigo pensar em outro jeito de 
    // checar a colisao sem mudar o objeto e ver se dá merda, na prática faz-se a ação inversa depois, então safe
    bool checkCollision(Tetromino &currentTetromino);
    bool placeTetromino(const Tetromino &currentTetromino, bool fallen); 
    int  normalizedY(int y) const;
    void clearFallingTetrominos();
    std::vector<std::vector<Cell>> &getGrid();

};

#endif