#include "game/board.hpp"
#include <stdlib.h>
#include "ServerManager.hpp"

const int TetrisBoard::WIDTH = 10;
const int TetrisBoard::HEIGHT = 16;

TetrisBoard::TetrisBoard(ServerManager &serverManager) : serverManager(serverManager)
{
    for (int y = 0; y < HEIGHT; ++y)
    {
        std::vector<std::shared_ptr<Cell>> row;

        for (int x = 0; x < WIDTH; ++x)
        {
            auto cell = std::make_shared<Cell>(x, y);
            row.push_back(cell);
        }

        grid.push_back(row);
    }
}

bool TetrisBoard::reachedTop()
{
    for (int y = 0; y < WIDTH; ++y)
    {
        if (grid.at(0).at(y)->isFixed())
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
            std::cout << (grid[x][y]->isFalling() ? " # " : (grid[x][y]->isEmpty()) ? " * "
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
            grid[x][y]->setEmpty();
        }
    }
}

std::vector<std::vector<std::shared_ptr<Cell>>> &TetrisBoard::getGrid()
{
    return grid;
}

bool TetrisBoard::checkCollision(Tetromino &currentTetromino, TetrisAction lastMove)
{
    currentTetromino.evolveStates(true, lastMove);

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
                    currentTetromino.evolveStates(false, lastMove);
                    std::cout << "bloqueei pois last move = " <<  TetrisActionToString(lastMove)  << "e passou dos limites"; 
                    return true;
                }

                if (grid[gridX][gridY]->isFixed())
                {
                    currentTetromino.evolveStates(false, lastMove);
                    std::cout << "bloqueei pois last move = " << TetrisActionToString(lastMove) << "e ia trombar em (" << gridX << ", " << gridY << ")"; 
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
                    grid[gridX][gridY]->setFixed(tetroColor);
                else
                    grid[gridX][gridY]->setFalling(tetroColor);
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
            if (grid[x][y]->isFalling())
            {
                grid[x][y]->setEmpty();
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
            if (grid[x][y]->isFixed())
            {
                grid[x][y]->setEmpty();
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
            if (grid[x][y]->isFixed())
                sumLine++;
        }

        // ------- Clear -------
        if (sumLine == WIDTH)
        {
            // 1 - Mudar o estado de todas as celulas daquela linha pra empty
            for (int y = 0; y < WIDTH; ++y)
            {
                grid[x][y]->setEmpty();
            }

            // 2 - Carregar todos os fixos dali pra cima "pra baixo"
            for (int x_clear = x; x_clear >= 1; --x_clear)
            {
                for (int y = 0; y < WIDTH; ++y)
                {

                    // Se o de cima for algum bloco fixo
                    if (grid[x_clear - 1][y]->isFixed())
                    {

                        // Setar o de baixo como fixo, com a cor do de cima
                        grid[x_clear][y]->setFixed(grid[x_clear - 1][y]->getColor());

                        // E deixar oq foi mudado como vazio
                        grid[x_clear - 1][y]->setEmpty();
                    }
                }
            }

            x++;
            numLinesCleared++;
        }
    }

    return numLinesCleared;
}

void TetrisBoard::broadcastBoardState()
{
    // Create a JSON object to hold the board data
    nlohmann::json boardJson;
    boardJson["width"] = WIDTH;
    boardJson["height"] = HEIGHT;

    nlohmann::json cells = nlohmann::json::array();

    // Iterate through the board's grid
    for (int y = 0; y < HEIGHT; ++y)
    {
        nlohmann::json row = nlohmann::json::array();
        for (int x = 0; x < WIDTH; ++x)
        {
            // Assume each Cell has a method getColorType() that returns an int
            CellColorType colorType = grid.at(y).at(x)->getColor();

            nlohmann::json cellObj;
            cellObj["c"] = colorType;
            row.push_back(cellObj);
        }
        cells.push_back(row);
    }

    boardJson["cells"] = cells;

    // Create a packet from the JSON
    Packet boardPacket(PacketType::GAME_SCREEN, boardJson, nullptr);

    // Send the packet to all connected players (broadcast)
    serverManager.send_packet(boardPacket);
}