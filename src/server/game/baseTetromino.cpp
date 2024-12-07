#include "game/baseTetromino.hpp"

const std::vector<CellColorType> Tetromino::tetromino_colors = {
    CellColorType::Red,
    CellColorType::Blue,
    CellColorType::Yellow,
    CellColorType::Green
};

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

int Tetromino::getLastMove() const
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

    case LEFT:
        y -= sign;
        break;
    case RIGHT:
        y += sign;
        break;
    case DOWN_FASTER:
        x += sign;
        break;

    default:
        break;
    }

    size_t rows = shape.size();
    size_t cols = shape[0].size();

    // Cria a matriz rotacionada
    std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows));

    if ((lastMove == ROT_LEFT && forward) || (lastMove == ROT_RIGHT && !forward))
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

    if ((lastMove == ROT_RIGHT && forward) || (lastMove == ROT_LEFT && !forward))
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
    lastMove = EMPTY;
    gravity = false;
}

void Tetromino::setLastAction(MOVES action)
{
    lastMove = action;
}

void Tetromino::dropGravity()
{
    gravity = true;
}