#include "game/board.hpp"
#include <stdlib.h>

const int TetrisBoard::WIDTH = 10;
const int TetrisBoard::HEIGHT = 16;

TetrisBoard::TetrisBoard() 
    : grid(HEIGHT, std::vector<Cell>())
{
    for (int x = 0; x < HEIGHT; ++x)
        for (int y = 0; y < WIDTH; ++y)        
            grid[x][y] = Cell(x, y);
}

bool TetrisBoard::reachedTop(){
    for(int y = 0; y < WIDTH; ++y){
        if(grid[0][y].isFixed())
            return true;
    }

    return false;
}

void TetrisBoard::printStatus()
{
    system("clear");

    // Imprimir status da board
    std::cout << "\nEstado da Board:" << std::endl;
    for (int x = 0; x < HEIGHT; x++)
    {
        for (int y = 0; y < WIDTH; y++)
        {
            std::cout << (grid[x][y].isFalling() ? " # " : (grid[x][y].isEmpty()) ? " * "
                                                                                  : " $ ");
        }
        std::cout << std::endl;
    }

    // Separador visual
    std::cout << std::string(40, '-') << std::endl;
}

void TetrisBoard::clear()
{
    for (int x = 0; x < HEIGHT; x++)
    {
        for (int y = 0; y < WIDTH; y++)
        {
            grid[x][y].setEmpty();
        }
    }
}
void TetrisBoard::handleInput(Tetromino &currentTetromino)
{
    /*sf::Event event;
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
                currentTetromino.setLastAction(LEFT);
                break;

            case sf::Keyboard::Right:
                currentTetromino.setLastAction(RIGHT);
                break;

            case sf::Keyboard::Up:
                currentTetromino.setLastAction(ROT_RIGHT);
                break;

            case sf::Keyboard::Down:
                currentTetromino.setLastAction(ROT_LEFT);
                break;

            case sf::Keyboard::Space:
                currentTetromino.setLastAction(DOWN_FASTER);
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
    */
}

std::vector<std::vector<Cell>> &TetrisBoard::getGrid()
{
    return grid;
}

void TetrisBoard::render()
{
    //Send to clients maybe?
}

bool TetrisBoard::checkCollision(Tetromino &currentTetromino)
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

int TetrisBoard::normalizedY(int y)
{

    y %= WIDTH;

    if (y < 0)
    {
        y += WIDTH;
    }

    return y;
}

// Integra o tetromino com o resto que já caiu
bool TetrisBoard::placeTetromino(const Tetromino &currentTetromino, bool bottom)
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
                CellColorType tetroColor = currentTetromino.getColor();

                if (bottom)
                    grid[gridX][gridY].setFixed(tetroColor);
                else
                    grid[gridX][gridY].setFalling(tetroColor);
            }
        }
    }

    return true;
}

void TetrisBoard::clearFallingTetrominos()
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

void TetrisBoard::clearFalledTetrominos()
{
    for (int x = 0; x < HEIGHT; ++x)
    {
        for (int y = 0; y < WIDTH; ++y)
        {
            if (grid[x][y].isFixed())
            {
                grid[x][y].setEmpty();
            }
        }
    }
}

int TetrisBoard::clearLines()
{
    int numLinesCleared = 0;

    // De baixo pra cima
    for (int x = HEIGHT - 1; x >= 0; --x)
    {
        int sumLine = 0;
        for (int y = 0; y < WIDTH; ++y)
        {
            if (grid[x][y].isFixed())
                sumLine++;
        }

        // ------- Clear -------
        if (sumLine == WIDTH)
        {
            // 1 - Mudar o estado de todas as celulas daquela linha pra empty
            for (int y = 0; y < WIDTH; ++y)
            {
                grid[x][y].setEmpty();
            }

            // 2 - Carregar todos os fixos dali pra cima "pra baixo"
            for (int x_clear = x; x_clear >= 1; --x_clear)
            {
                for (int y = 0; y < WIDTH; ++y)
                {

                    // Se o de cima for algum bloco fixo
                    if (grid[x_clear - 1][y].isFixed())
                    {

                        // Setar o de baixo como fixo, com a cor do de cima
                        grid[x_clear][y].setFixed(grid[x_clear - 1][y].getColor());

                        // E deixar oq foi mudado como vazio
                        grid[x_clear - 1][y].setEmpty();
                    }
                }
            }

            x++;
            numLinesCleared++;
        }
    }

    return numLinesCleared;
}