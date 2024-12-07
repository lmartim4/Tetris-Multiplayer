#include "game/baseTetromino.hpp"

const std::vector<CellColorType> Tetromino::tetromino_colors = {
    CellColorType::Red,
    CellColorType::Blue,
    CellColorType::Yellow,
    CellColorType::Green};

const std::vector<char> Tetromino::types = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};

int Tetromino::getX() const
{
    return x;
}

int Tetromino::getY() const
{
    return y;
}

CellColorType Tetromino::getColor() const
{
    return color;
}

TetrisAction Tetromino::getLastMove() const
{
    return lastMove;
}

bool Tetromino::getGravity() const
{
    return gravity;
}

const std::vector<std::vector<int>> &Tetromino::getShape() const
{
    return shape;
}

void Tetromino::evolveStates(bool forward)
{
    int sign = (forward) ? 1 : -1;

    if (gravity)
    {
        x += sign;
    }

    switch (lastMove)
    {

    case TetrisAction::LEFT:
        y -= sign;
        break;
    case TetrisAction::RIGHT:
        y += sign;
        break;
    case TetrisAction::DROP_FASTER:
        x += sign;
        break;

    default:
        break;
    }

    size_t rows = shape.size();
    size_t cols = shape[0].size();

    // Cria a matriz rotacionada
    std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows));

    if ((lastMove == TetrisAction::ROTATE_LEFT && forward) || (lastMove == TetrisAction::ROTATE_RIGHT && !forward))
    {

        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                rotated[j][rows - i - 1] = shape[i][j];
            }
        }

        shape = rotated;
    }

    if ((lastMove == TetrisAction::ROTATE_RIGHT && forward) || (lastMove == TetrisAction::ROTATE_LEFT && !forward))
    {
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                rotated[cols - j - 1][i] = shape[i][j];
            }
        }

        shape = rotated;
    }
}

void Tetromino::updateStates()
{
    lastMove = TetrisAction::EMPTY;
    gravity = false;
}

void Tetromino::setLastAction(TetrisAction action)
{
    lastMove = action;
}

void Tetromino::dropGravity()
{
    gravity = true;
}