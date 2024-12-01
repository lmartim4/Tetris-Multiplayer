#include "../include/board.hpp"
#include <stdlib.h>

Board::Board(int W, int H, int cell_size) : WIDTH(W), HEIGHT(H), CELL_SIZE(cell_size), grid(HEIGHT, std::vector<Cell>(WIDTH))
{
    for (int x = 0; x < HEIGHT; ++x)
    {
        for (int y = 0; y < WIDTH; ++y)
        {
            grid[x][y] = Cell(cell_size, x, y);
        }
    }
}

std::vector<std::vector<Cell>> &Board::getGrid()
{
    return grid;
}

void Board::render(sf::RenderWindow &window)
{
    for (int x = 0; x < HEIGHT; ++x)
    {
        for (int y = 0; y < WIDTH; ++y)
        {
            window.draw(grid[x][y].getCell());
        }
    }
}

// Checks if the piece hit the bottom of the pit (next grid y position is bigger than
// the size of the board OR the next y position of any part of the shape is another block
// on the "pile" of fallen blocks)
bool Board::checkCollision(Tetromino &currentTetromino)
{
    currentTetromino.evolveStates(true);

    const auto &shape = currentTetromino.getShape();
    int tetrominoX = currentTetromino.getX();
    int tetrominoY = currentTetromino.getY();
    int lastMove = currentTetromino.getLastMove();

    // Percorre a matriz de "shape" do Tetromino
    for (size_t x = 0; x < shape.size(); ++x)
    {
        for (size_t y = 0; y < shape[x].size(); ++y)
        {
            if (shape[x][y] != 0)
            { // Se a célula faz parte do Tetromino
                int gridX = tetrominoX + x;
                int gridY = normalizedY(tetrominoY + y);

                if (gridX >= HEIGHT || gridX < 0)                    
                {
                    currentTetromino.evolveStates(false);
                    return true;
                }

                if (grid[gridX][gridY].isFixed())
                {
                    currentTetromino.evolveStates(false);
                    return true;
                }

            }
        }
    }

    return false;
}

int Board::normalizedY(int y) const
{

    y %= WIDTH;

    if (y < 0)
    {
        y += WIDTH;
    }

    return y;
}

// Integra o tetromino com o resto que já caiu
bool Board::placeTetromino(const Tetromino &currentTetromino, bool bottom)
{

    if (bottom)
        std::cout << "Fui setado como fixo" << std::endl;
    // Use os métodos virtuais de Tetromino aqui
    const auto &shape = currentTetromino.getShape();

    for (size_t x = 0; x < shape.size(); ++x)
    {
        for (size_t y = 0; y < shape[x].size(); ++y)
        {
            if (shape[x][y] != 0)
            { // Verifica se a célula faz parte do Tetromino
                int gridX = currentTetromino.getX() + x;
                int gridY = normalizedY(currentTetromino.getY() + y);
                sf::Color tetroColor = currentTetromino.getColor();

                // std::cout << "Fallen = " << fallen << std::endl;
                if (bottom)
                {
                    std::cout << " (" << gridX << ", " << gridY << ") ";
                    grid[gridX][gridY].setFixed(tetroColor);
                }
                else
                {
                    grid[gridX][gridY].setFalling(tetroColor);
                }
            }
        }
    }

    if (bottom)
        std::cout << std::endl;

    return true; // Substitua pela lógica de posicionamento
}

void Board::clearFallingTetrominos()
{
    for (int x = 0; x < HEIGHT; ++x)
    {
        for (int y = 0; y < WIDTH; ++y)
        {
            // Limpa todas as células ocupadas por Tetrominos em queda (ID > 0)
            if (grid[x][y].isFalling())
            {
                grid[x][y].setEmpty();
            }
        }
    }
}
