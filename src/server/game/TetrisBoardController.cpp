
#include "TetrisBoardController.hpp"
#include "Cell.hpp"

#include <iostream>

TetrisBoardController::TetrisBoardController(std::shared_ptr<TetrisBoard> board)
    : board(board)
{
}

CollisionType TetrisBoardController::checkCollision(std::shared_ptr<Tetromino> currentTetromino, TetrisAction action) const
{
    currentTetromino->evolveStates(true, action);

    const auto &shape = currentTetromino->getShape();
    auto &grid = board->getGrid();

    int gridX, gridY;

    for (size_t x = 0; x < shape.size(); ++x)
    {
        for (size_t y = 0; y < shape[x].size(); ++y)
        {
            if (shape[x][y] != 0)
            {
                gridX = currentTetromino->getCoordinate().x + x;
                gridY = board->getNormalizedY(currentTetromino->getCoordinate().y + y);

                // Check for out-of-bounds
                if (gridX < 0 || gridX >= board->getHeight())
                {
                    currentTetromino->evolveStates(false, action);
                    return CollisionType::OUT_OF_BOUNDS;
                }

                // Check for fallen blocks
                if (grid[gridX][gridY]->getState() == CellState::FALLEN)
                {
                    currentTetromino->evolveStates(false, action);
                    return CollisionType::FALLEN_OR_BOUNDARY;
                }

                // Check for collision with other falling Tetromino
                if (grid[gridX][gridY]->getState() == CellState::FALLING && grid[gridX][gridY]->getPieceId() != currentTetromino->getId())
                {
                    currentTetromino->evolveStates(false, action);
                    return CollisionType::FALLING_OTHER;
                }
            }
        }
    }

    // No collision
    return CollisionType::NONE;
}

void TetrisBoardController::placeTetromino(const std::shared_ptr<Tetromino> currentTetromino, bool fallen)
{
    const auto &shape = currentTetromino->getShape();
    auto &grid = board->getGrid();

    CellState newState = fallen ? CellState::FALLEN : CellState::FALLING;
    int myId = currentTetromino->getId();

    int baseX = currentTetromino->getCoordinate().x;
    int baseY = currentTetromino->getCoordinate().y;

    for (size_t x = 0; x < shape.size(); x++)
    {
        for (size_t y = 0; y < shape[x].size(); y++)
        {
            if (shape[x][y] != 0)
            {

                int gridX = baseX + static_cast<int>(x);
                int gridY = board->getNormalizedY(baseY + static_cast<int>(y));

                grid[gridX][gridY]->setState(newState);
                grid[gridX][gridY]->setColor(currentTetromino->getColor());
                grid[gridX][gridY]->setPieceId(myId);
            }
        }
    }
}

int TetrisBoardController::clearFullLines()
{
    auto &grid = board->getGrid();
    int width = board->getWidth();
    int height = board->getHeight();

    int writeRow = height - 1;
    int numLinesCleared = 0;

    // Go from bottom row up to top
    for (int readRow = height - 1; readRow >= 0; --readRow)
    {
        // Check if readRow is full
        bool isFull = true;
        for (int y = 0; y < width; ++y)
        {
            if (grid[readRow][y]->getState() != CellState::FALLEN)
            {
                isFull = false;
                break;
            }
        }

        if (!isFull)
        {
            // Copy readRow -> writeRow if they differ
            if (readRow != writeRow)
            {
                for (int y = 0; y < width; ++y)
                {
                    // Copy color & state
                    grid[writeRow][y]->setState(grid[readRow][y]->getState());
                    grid[writeRow][y]->setColor(grid[readRow][y]->getColor());
                }
            }
            writeRow--;
        }
        else
        {
            numLinesCleared++;
        }
    }

    // Fill the remaining rows above writeRow with empty
    for (int row = writeRow; row >= 0; --row)
    {
        for (int y = 0; y < width; ++y)
        {
            grid[row][y]->setEmpty();
        }
    }

    return numLinesCleared;
}

void TetrisBoardController::clearFallingTetromino(const std::shared_ptr<Tetromino> currentTetromino)
{
    const auto &shape = currentTetromino->getShape();
    auto &grid = board->getGrid();
    auto tetroColor = currentTetromino->getColor();

    int baseX = currentTetromino->getCoordinate().x;
    int baseY = currentTetromino->getCoordinate().y;
    int myId = currentTetromino->getId();

    for (size_t x = 0; x < shape.size(); x++)
    {
        for (size_t y = 0; y < shape[x].size(); y++)
        {
            if (shape[x][y] != 0)
            {
                int gridX = baseX + static_cast<int>(x);
                int gridY = board->getNormalizedY(baseY + static_cast<int>(y));

                if (grid[gridX][gridY]->getPieceId() == myId)
                {
                    grid[gridX][gridY]->setEmpty();
                }
            }
        }
    }
}

void TetrisBoardController::clearFallenTetrominos()
{
    for (int x = 0; x < board->getHeight(); x++)
        for (int y = 0; y < board->getWidth(); y++)
            if (board->getGrid()[x][y]->getState() == FALLEN)
                board->getGrid()[x][y]->setEmpty();
}