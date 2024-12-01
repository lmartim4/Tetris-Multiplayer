#include "../include/board.hpp"
#include <stdlib.h>

const int Board::WIDTH = 10;
const int Board::HEIGHT = 20;
const int Board::CELL_SIZE = 40;

Board::Board() : window(sf::VideoMode(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE), "Tetris"),
                 grid(HEIGHT, std::vector<Cell>(WIDTH))
{
    for (int x = 0; x < HEIGHT; ++x)
    {
        for (int y = 0; y < WIDTH; ++y)
        {
            grid[x][y] = Cell(CELL_SIZE, x, y);
        }
    }
}

void Board::printStatus()
{
    system("clear");

    // Imprimir status da board
    std::cout << "\nEstado da Board:" << std::endl;
    for (int x = 0; x < HEIGHT; x++)
    {
        for (int y = 0; y < WIDTH; y++)
        {
            std::cout << (grid[x][y].isFalling() ? " # " : (grid[x][y].isEmpty()) ? " * " : " $ ");
        }
        std::cout << std::endl;
    }
    
    // Separador visual
    std::cout << std::string(40, '-') << std::endl; 
}

bool Board::windowIsOpen()
{
    return window.isOpen();
}

void Board::handleInput(Tetromino &currentTetromino)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {

        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::Left: 
                currentTetromino.action(LEFT);
                break;

            case sf::Keyboard::Right: 
                currentTetromino.action(RIGHT);
                break;

            case sf::Keyboard::Up: 
                currentTetromino.action(ROT_RIGHT);
                break;

            case sf::Keyboard::Down:
                currentTetromino.action(ROT_LEFT);
                break;

            case sf::Keyboard::Space:
                currentTetromino.action(DOWN_FASTER);
                break;

                // case sf::Keyboard::Enter: // Drop it to the ground !
                //     currentTetromino->dropToTheGround();
                //     break;

            case sf::Keyboard::Escape: 
                window.close();
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
    }
}

std::vector<std::vector<Cell>> &Board::getGrid()
{
    return grid;
}

void Board::render()
{
    window.clear();

    for (int x = 0; x < HEIGHT; ++x)
    {
        for (int y = 0; y < WIDTH; ++y)
        {
            window.draw(grid[x][y].getCell());
        }
    }

    window.display();
}

bool Board::checkCollision(Tetromino &currentTetromino)
{
    currentTetromino.evolveStates(true);

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

int Board::normalizedY(int y) 
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

    const auto &shape = currentTetromino.getShape();

    for (size_t x = 0; x < shape.size(); ++x)
    {
        for (size_t y = 0; y < shape[x].size(); ++y)
        {   
            if (shape[x][y] != 0)
            { 
                int gridX = currentTetromino.getX() + x;
                int gridY = normalizedY(currentTetromino.getY() + y);
                sf::Color tetroColor = currentTetromino.getColor();

                if (bottom)
                    grid[gridX][gridY].setFixed(tetroColor);
                else
                    grid[gridX][gridY].setFalling(tetroColor);
            }
        }
    }

    return true; 
}

void Board::clearFallingTetrominos()
{
    for (int x = 0; x < HEIGHT; ++x)
    {
        for (int y = 0; y < WIDTH; ++y)
        {
            if (grid[x][y].isFalling())
            {
                grid[x][y].setEmpty();
            }
        }
    }
}
