#include "game/TetrisBoard.hpp"
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

    for (size_t x = 0; x < shape.size(); ++x)
    {
        for (size_t y = 0; y < shape[x].size(); ++y)
        {
            if (shape[x][y] != 0)
            {
                int gridX = tetrominoX + x;
                int gridY = normalizedY(tetrominoY + y);

                if (gridX >= HEIGHT || gridX < 0)
                {
                    currentTetromino.evolveStates(false, lastMove);
                    return true;
                }

                if (grid[gridX][gridY]->isFixed())
                {
                    currentTetromino.evolveStates(false, lastMove);
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
        y += WIDTH;

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
    if (!gridsAreEqual(grid, lastBroadcastedGrid))
    {
        updateLastBroadcastedGrid();                     // Step 1: Update lastBroadcastedGrid to match the current grid
        nlohmann::json boardJson = constructBoardJson(); // Step 2: Construct the JSON object
        sendBoardState(boardJson);                       // Step 3: Broadcast the JSON object
    }
}

// Updates lastBroadcastedGrid with a deep copy of the current grid
void TetrisBoard::updateLastBroadcastedGrid()
{
    lastBroadcastedGrid.clear();
    lastBroadcastedGrid.reserve(grid.size());

    for (const auto &row : grid)
    {
        std::vector<std::shared_ptr<Cell>> newRow;
        newRow.reserve(row.size());

        for (const auto &cellPtr : row)
        {
            if (cellPtr)
            {
                newRow.emplace_back(std::make_shared<Cell>(*cellPtr));
            }
            else
            {
                newRow.emplace_back(nullptr);
            }
        }

        lastBroadcastedGrid.emplace_back(std::move(newRow));
    }
}

// Constructs a JSON representation of the board's current state
nlohmann::json TetrisBoard::constructBoardJson() const
{
    nlohmann::json boardJson;
    boardJson["width"] = WIDTH;
    boardJson["height"] = HEIGHT;

    nlohmann::json cells = nlohmann::json::array();

    for (int y = 0; y < HEIGHT; ++y)
    {
        nlohmann::json row = nlohmann::json::array();
        for (int x = 0; x < WIDTH; ++x)
        {
            CellColorType colorType = grid.at(y).at(x)->getColor();

            nlohmann::json cellObj;
            cellObj["c"] = colorType;
            row.push_back(cellObj);
        }
        cells.push_back(row);
    }

    boardJson["cells"] = cells;
    return boardJson;
}

// Sends the JSON board state using the server manager
void TetrisBoard::sendBoardState(const nlohmann::json &boardJson) const
{
    serverManager.send_packet(Packet(PacketType::GAME_SCREEN, boardJson, nullptr));
}

// Implementation of gridsAreEqual
bool TetrisBoard::gridsAreEqual(const std::vector<std::vector<std::shared_ptr<Cell>>> &grid1,
                                const std::vector<std::vector<std::shared_ptr<Cell>>> &grid2) const
{
    if (grid1.size() != grid2.size())
        return false;

    for (size_t i = 0; i < grid1.size(); ++i)
    {
        if (grid1[i].size() != grid2[i].size())
            return false;

        for (size_t j = 0; j < grid1[i].size(); ++j)
        {
            // Handle nullptrs if cells can be null
            if (grid1[i][j] == nullptr && grid2[i][j] == nullptr)
                continue;
            if ((grid1[i][j] == nullptr) != (grid2[i][j] == nullptr))
                return false;
            if (*grid1[i][j] != *grid2[i][j])
                return false;
        }
    }

    return true;
}