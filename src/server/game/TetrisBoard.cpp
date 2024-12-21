#include "game/TetrisBoard.hpp"
#include "TetrisBoard.hpp"

TetrisBoard::TetrisBoard(int h, int w) : Debuggable("Tetris Board")
{
    HEIGHT = h;
    WIDTH = w;

    console_log("Initializing Tetris-Board");
    for (int y = 0; y < HEIGHT; ++y)
    {
        std::vector<std::shared_ptr<GameCell>> row;

        for (int x = 0; x < WIDTH; ++x)
        {
            auto cell = std::make_shared<GameCell>(x, y);
            row.push_back(cell);
        }

        grid.push_back(row);
    }
}

bool TetrisBoard::reachedTop() const
{
    for (int y = 0; y < WIDTH; y++)
        if (grid.at(0).at(y)->getState() == CELL_FALLEN)
            return true;

    return false;
}

void TetrisBoard::printStatus() const
{
    system("clear");

    std::cout << "\nEstado da Board:" << std::endl;
    for (int x = 0; x < HEIGHT; x++)
    {
        for (int y = 0; y < WIDTH; y++)
            std::cout << (grid[x][y]->getState() == CELL_FALLING ? " # " : (grid[x][y]->getState() == CELL_EMPTY) ? " * "
                                                                                                                  : " $ ");
        std::cout << std::endl;
    }

    std::cout << std::string(40, '-') << std::endl;
}

void TetrisBoard::clear() const
{
    for (int x = 0; x < HEIGHT; x++)
        for (int y = 0; y < WIDTH; y++)
            grid[x][y]->setEmpty();
}

bool TetrisBoard::tryMove(Tetromino &currentTetromino, TetrisAction lastMove) const
{
    currentTetromino.evolveStates(true, lastMove);

    const auto &shape = currentTetromino.getShape();

    for (size_t x = 0; x < shape.size(); ++x)
        for (size_t y = 0; y < shape[x].size(); ++y)
            if (shape[x][y] != 0)
            {
                int gridX = currentTetromino.getCoordinate().x + x;
                int gridY = getNormalizedY(currentTetromino.getCoordinate().y + y);

                if (gridX >= HEIGHT || gridX < 0 || grid[gridX][gridY]->getState() == CELL_FALLEN)
                {
                    currentTetromino.evolveStates(false, lastMove);
                    return true;
                }
            }

    return false;
}

int TetrisBoard::getNormalizedY(int y) const
{
    y %= WIDTH;

    if (y < 0)
        y += WIDTH;

    return y;
}

void TetrisBoard::placeTetromino(const Tetromino &currentTetromino, bool bottom)
{
    const auto &shape = currentTetromino.getShape();

    for (size_t x = 0; x < shape.size(); x++)
        for (size_t y = 0; y < shape[x].size(); y++)
            if (shape[x][y] != 0)
            {
                int gridX = currentTetromino.getCoordinate().x + x;
                int gridY = getNormalizedY(currentTetromino.getCoordinate().y + y);
                CellColorType tetroColor = currentTetromino.getColor();

                if (bottom)
                {
                    if (grid[gridX][gridY]->getState() != CELL_FALLEN)
                    {
                        grid[gridX][gridY]->setFixed(tetroColor);
                    }
                }
                else
                {
                    if (grid[gridX][gridY]->getState() != CELL_FALLING)
                    {
                        grid[gridX][gridY]->setFalling(tetroColor);
                    }
                }
            }
}

void TetrisBoard::clearFallingTetrominos()
{
    for (int x = 0; x < HEIGHT; x++)
        for (int y = 0; y < WIDTH; y++)
            if (grid[x][y]->getState() == CELL_FALLING)
                grid[x][y]->setEmpty();
}

void TetrisBoard::clearFalledTetrominos()
{
    for (int x = 0; x < HEIGHT; x++)
        for (int y = 0; y < WIDTH; y++)
            if (grid[x][y]->getState() == CELL_FALLEN)
                grid[x][y]->setEmpty();
}

int TetrisBoard::clearLines() const
{
    int numLinesCleared = 0;

    // De baixo pra cima
    for (int x = HEIGHT - 1; x >= 0; --x)
    {
        int sumLine = 0;
        for (int y = 0; y < WIDTH; y++)
        {
            if (grid[x][y]->getState() == CELL_FALLEN)
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
                    if (grid[x_clear - 1][y]->getState() == CELL_FALLEN)
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

nlohmann::json TetrisBoard::constructBoardJsonToBroadcast() const
{
    nlohmann::json boardJson;
    boardJson["width"] = WIDTH;
    boardJson["height"] = HEIGHT;

    nlohmann::json cells = nlohmann::json::array();

    for (int x = 0; x < HEIGHT; ++x)
    {
        nlohmann::json row = nlohmann::json::array();

        for (int y = 0; y < WIDTH; ++y)
        {
            CellColorType colorType = grid.at(x).at(y)->getColor();

            nlohmann::json cellObj;
            cellObj["c"] = colorType;
            row.push_back(cellObj);
        }
        cells.push_back(row);
    }

    boardJson["cells"] = cells;
    return boardJson;
}