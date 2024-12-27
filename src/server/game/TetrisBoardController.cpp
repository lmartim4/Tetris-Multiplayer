
#include "TetrisBoardController.hpp"
#include "Cell.hpp"
#include <iostream>

TetrisBoardController::TetrisBoardController(TetrisBoard &board)
    : board(board)
{
}

bool TetrisBoardController::checkCollision(Tetromino &currentTetromino,
                                           TetrisAction action) const
{
    currentTetromino.evolveStates(true, action);

    const auto &shape = currentTetromino.getShape();

    for (size_t x = 0; x < shape.size(); ++x)
        for (size_t y = 0; y < shape[x].size(); ++y)
            if (shape[x][y] != 0)
            {
                int gridX = currentTetromino.getCoordinate().x + x;
                int gridY = board.getNormalizedY(currentTetromino.getCoordinate().y + y);

                if (gridX >= board.getHeight() || gridX < 0 || board.getGrid()[gridX][gridY]->getState() == FALLEN)
                {
                    currentTetromino.evolveStates(false, action);
                    return true;
                }
            }

    return false;
}

void TetrisBoardController::placeTetromino(const Tetromino &currentTetromino,
                                           bool fallen)
{
    const auto &shape = currentTetromino.getShape();

    for (size_t x = 0; x < shape.size(); x++)
        for (size_t y = 0; y < shape[x].size(); y++)
            if (shape[x][y] != 0)
            {
                int gridX = currentTetromino.getCoordinate().x + x;
                int gridY = board.getNormalizedY(currentTetromino.getCoordinate().y + y);
                CellColor tetroColor = currentTetromino.getColor();

                if (fallen)
                {
                    if (board.getGrid()[gridX][gridY]->getState() != CellState::FALLEN)
                    {
                        board.getGrid()[gridX][gridY]->setColor(tetroColor);
                        board.getGrid()[gridX][gridY]->setState(CellState::FALLEN);
                    }
                }
                else
                {
                    if (board.getGrid()[gridX][gridY]->getState() != FALLING)
                    {
                        board.getGrid()[gridX][gridY]->setState(CellState::FALLING);
                        board.getGrid()[gridX][gridY]->setColor(tetroColor);
                    }
                }
            }
}

int TetrisBoardController::clearFullLines()
{
    int numLinesCleared = 0;

    // De baixo pra cima
    for (int x = board.getHeight() - 1; x >= 0; --x)
    {
        int sumLine = 0;
        for (int y = 0; y < board.getWidth(); y++)
        {
            if (board.getGrid()[x][y]->getState() == FALLEN)
                sumLine++;
        }

        // ------- Clear -------
        if (sumLine == board.getWidth())
        {
            // 1 - Mudar o estado de todas as celulas daquela linha pra empty
            for (int y = 0; y < board.getWidth(); ++y)
            {
                board.getGrid()[x][y]->setEmpty();
            }

            // 2 - Carregar todos os fixos dali pra cima "pra baixo"
            for (int x_clear = x; x_clear >= 1; --x_clear)
            {
                for (int y = 0; y < board.getWidth(); ++y)
                {
                    // Se o de cima for algum bloco fixo
                    if (board.getGrid()[x_clear - 1][y]->getState() == FALLEN)
                    {
                        // Setar o de baixo como fixo, com a cor do de cima
                        board.getGrid()[x_clear][y]->setColor(board.getGrid()[x_clear - 1][y]->getColor());
                        board.getGrid()[x_clear][y]->setState(CellState::FALLEN);

                        // E deixar oq foi mudado como vazio
                        board.getGrid()[x_clear - 1][y]->setEmpty();
                    }
                }
            }

            x++;
            numLinesCleared++;
        }
    }
    return numLinesCleared;
}

void TetrisBoardController::clearFallingTetrominos()
{
    for (int x = 0; x < board.getHeight(); x++)
        for (int y = 0; y < board.getWidth(); y++)
            if (board.getGrid()[x][y]->getState() == FALLING)
                board.getGrid()[x][y]->setEmpty();
}

void TetrisBoardController::clearFallenTetrominos()
{
    for (int x = 0; x < board.getHeight(); x++)
        for (int y = 0; y < board.getWidth(); y++)
            if (board.getGrid()[x][y]->getState() == FALLEN)
                board.getGrid()[x][y]->setEmpty();
}