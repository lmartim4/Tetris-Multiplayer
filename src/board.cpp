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

const std::vector<std::tuple<int, int, sf::Color>> &Board::getRenderCells() const
{
    return renderCache; // Retorna o cache já calculado
}

void Board::render(sf::RenderWindow &window, const Tetromino &currentTetromino)
{

    // system("CLS");
    for (int x = 0; x < HEIGHT; ++x)
    {
        for (int y = 0; y < WIDTH; ++y)
        {

            // grid[x][y].setPosition(y* CELL_SIZE, x * CELL_SIZE);

            if (!grid[x][y].isFalling())
            { // If there's no block here
                // std::cout << x << ", " << y << " Desenhado como vazio" << std::endl;

                window.draw(grid[x][y].getCell());

                // std::cout << "Desenhado " << grid[x][y].isFalling() << " em " << x << ", " << y << std::endl;
            }
        }
    }
    // std::cout << "Cabou render dos vazios" << std::endl;

    // Renderizar o Tetromino atual
    const auto &shape = currentTetromino.getShape();
    int tetrominoX = currentTetromino.getX();
    int tetrominoY = currentTetromino.getY();

    // sf::RectangleShape block(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    // block.setFillColor(currentTetromino.getColor()); // Cor do Tetromino atual
    // block.setOutlineThickness(1);
    // block.setOutlineColor(sf::Color::Black); // Contorno para clareza

    for (size_t x = 0; x < shape.size(); ++x)
    {
        for (size_t y = 0; y < shape[x].size(); ++y)
        {
            if (shape[x][y] != 0)
            { // Se a célula faz parte do Tetromino
                // grid[i][j].setPosition((size_t)(normalizedX(tetrominoX + j)) * CELL_SIZE, (tetrominoY + i) * CELL_SIZE);
                grid[x][y].setFalling(currentTetromino.getColor());
                window.draw(grid[tetrominoX + x][normalizedY(tetrominoY + y )].getCell());
            }
        }
    }
}

// Checks if the piece hit the bottom of the pit (next grid y position is bigger than
// the size of the board OR the next y position of any part of the shape is another block
// on the "pile" of fallen blocks)
bool Board::checkCollision(const Tetromino &currentTetromino)
{
    const auto &shape = currentTetromino.getShape();
    int tetrominoX = currentTetromino.getX();
    int tetrominoY = currentTetromino.getY();

    // Percorre a matriz de "shape" do Tetromino
    for (size_t x = 0; x < shape.size(); ++x)
    {
        for (size_t y = 0; y < shape[x].size(); ++y)
        {
            if (shape[x][y] != 0)
            { // Se a célula faz parte do Tetromino
                int gridX = tetrominoX + x;
                int gridY = normalizedY(tetrominoY + y);

                // Verifica se o bloco atingiu o fundo do tabuleiro
                if (gridX + 1 >= HEIGHT)
                {
                    std::cout << "Ta no fundo" << std::endl;
                    return true; // Colisão com o fundo
                }

                // Verifica se o bloco atingiu outro bloco na "pilha"
                if (gridX + 1 >= 0 && grid[gridX + 1][gridY].isFixed())
                {
                    std::cout << "Bateu em outro no fundo" << std::endl;
                    return true; // Colisão com a pilha de blocos
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
                    grid[gridX][gridY].setFixed(tetroColor);
                    // std::cout << "Fui setado como fixo" << std::endl;
                }
                else
                {
                    grid[gridX][gridY].setFalling(tetroColor);
                }
            }
        }
    }

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
