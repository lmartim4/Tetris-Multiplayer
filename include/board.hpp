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

    // Nao entendi completamente
    // Cache para otimizar a renderização
    std::vector<std::tuple<int, int, sf::Color>> renderCache;

    // Atualiza o cache de renderização
    void updateRenderCache();

public:
    Board(int W, int H, int cell_size);

    // Nao entendi completamente
    // Retorna as células a serem renderizadas
    const std::vector<std::tuple<int, int, sf::Color>> &getRenderCells() const;

    void render(sf::RenderWindow &window,const Tetromino &currentTetromino) ;

    bool checkCollision(const Tetromino &currentTetromino);
    bool placeTetromino(const Tetromino &currentTetromino, bool fallen); 
    int  normalizedY(int y) const;
    void clearFallingTetrominos();
    std::vector<std::vector<Cell>> &getGrid();

};

#endif