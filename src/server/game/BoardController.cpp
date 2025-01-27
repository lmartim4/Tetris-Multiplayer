
#include "BoardController.hpp"
#include "TetrominoController.hpp"

#include "Cell.hpp"

#include <iostream>

BoardController::BoardController(std::shared_ptr<TetrisBoard> board)
    : board(board)
{
}

void BoardController::printMatrix(const std::vector<std::vector<int>> &matrix, const std::string &label) const
{
    std::cout << label << ":\n";
    for (const auto &row : matrix)
    {
        for (const auto &cell : row)
            std::cout << cell << " ";
        std::cout << "\n";
    }
    std::cout << std::endl;
}

CollisionType BoardController::checkCollision(std::shared_ptr<Tetromino> currentTetromino, TetrisAction action, std::shared_ptr<TetrominoController> tetrominoController) const
{

    /*const std::vector<std::vector<int>> before = currentTetromino->getShape()->getShape();
    printMatrix(before, "Before");*/
    tetrominoController->transform(currentTetromino, action, true);
    const std::vector<std::vector<int>> shape = currentTetromino->getShape()->getShape();
    // printMatrix(shape, "After");

    std::shared_ptr<TetrisBoard> b = board.lock();
    if (!b)
        throw std::runtime_error("Where is my board?");

    std::vector<std::vector<std::shared_ptr<Cell>>> &grid = b->getGrid();

    CellState state = CellState::EMPTY;
    CollisionType worstCollision = CollisionType::NONE;

    int gridX, gridY;

    for (size_t x = 0; x < shape.size(); x++)
    {
        for (size_t y = 0; y < shape[x].size(); y++)
        {
            // skip tetrominios empty slots
            if (shape[x][y] == 0)
                continue;

            // gridX might be out of a vector value but gridY is always protected
            gridX = currentTetromino->getCoordinate().getX() + x;
            gridY = b->getNormalizedY(currentTetromino->getCoordinate().getY() + y);

            if (gridX < 0 || gridX >= b->getHeight())
            {
                worstCollision = CollisionType::GROUND;
                continue;
            }

            state = grid[gridX][gridY]->getState();

            if (state == CellState::FALLEN)
            {
                if (worstCollision < CollisionType::FALLEN_FIXED)
                    worstCollision = CollisionType::FALLEN_FIXED;
            }
            else if (state == CellState::FALLING && grid[gridX][gridY]->getPieceId() != currentTetromino->getId())
            {
                if (worstCollision < CollisionType::FALLING_OTHER)
                    worstCollision = CollisionType::FALLING_OTHER;
            }
        }
    }

    tetrominoController->transform(currentTetromino, action, false);
    return worstCollision;
}
void BoardController::setCellState(int ownerID, const std::shared_ptr<Tetromino> currentTetromino, CellState state)
{
    std::shared_ptr<TetrisBoard> b = board.lock();

    if (!b)
        throw std::runtime_error("Where is my board?");

    std::shared_ptr<TetrominoShape> tshape = currentTetromino->getShape();
    const std::vector<std::vector<int>> shape = tshape->getShape();

    auto &grid = b->getGrid();

    int myId = currentTetromino->getId();

    int baseX = currentTetromino->getCoordinate().getX();
    int baseY = currentTetromino->getCoordinate().getY();

    for (size_t x = 0; x < shape.size(); x++)
    {
        for (size_t y = 0; y < shape[x].size(); y++)
        {
            if (shape[x][y] == 0)
                continue;

            int gridX = baseX + static_cast<int>(x);
            int gridY = b->getNormalizedY(baseY + static_cast<int>(y));

            grid[gridX][gridY]->setOwnerID(ownerID);
            grid[gridX][gridY]->setState(state);
            grid[gridX][gridY]->setColor(currentTetromino->getColor());
            grid[gridX][gridY]->setPieceId(myId);
        }
    }
}

int BoardController::findAndClearFullLines()
{
    std::shared_ptr<TetrisBoard> b = board.lock();

    if (!b)
        throw std::runtime_error("Where is my board?");

    auto &grid = b->getGrid();
    int width = b->getWidth();
    int height = b->getHeight();

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
                for (int x = 0; x < width; ++x)
                {
                    // Copy the properties of FALLEN cells; empty cells stay empty
                    CellState readState = grid[readRow][x]->getState();
                    if (readState == CellState::FALLEN)
                    {
                        grid[writeRow][x]->setState(CellState::FALLEN);
                        grid[writeRow][x]->setColor(grid[readRow][x]->getColor());
                    }
                    else
                    {
                        if (readState != CellState::FALLING)
                            grid[writeRow][x]->setEmpty();
                    }
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
        for (int y = 0; y < width; ++y)
            if (grid[row][y]->getState() != CellState::FALLING)
                grid[row][y]->setEmpty();

    return numLinesCleared;
}

void BoardController::clearFallingTetromino(const std::shared_ptr<Tetromino> currentTetromino)
{
    std::shared_ptr<TetrisBoard> b = board.lock();

    if (!b)
        throw std::runtime_error("Where is my board?");

    std::shared_ptr<TetrominoShape> tshape = currentTetromino->getShape();
    const std::vector<std::vector<int>> shape = tshape->getShape();

    auto &grid = b->getGrid();
    auto tetroColor = currentTetromino->getColor();

    int baseX = currentTetromino->getCoordinate().getX();
    int baseY = currentTetromino->getCoordinate().getY();
    int myId = currentTetromino->getId();

    for (size_t x = 0; x < shape.size(); x++)
    {
        for (size_t y = 0; y < shape[x].size(); y++)
        {
            if (shape[x][y] == 0)
                continue;

            int gridX = baseX + static_cast<int>(x);
            int gridY = b->getNormalizedY(baseY + static_cast<int>(y));

            if (grid[gridX][gridY]->getPieceId() == myId)
            {
                grid[gridX][gridY]->setEmpty();
            }
        }
    }
}

void BoardController::clearFallenTetrominos()

{
    std::shared_ptr<TetrisBoard> b = board.lock();

    if (!b)
        throw std::runtime_error("Where is my board?");

    for (int x = 0; x < b->getHeight(); x++)
        for (int y = 0; y < b->getWidth(); y++)
            if (b->getGrid()[x][y]->getState() == CellState::FALLEN)
                b->getGrid()[x][y]->setEmpty();
}