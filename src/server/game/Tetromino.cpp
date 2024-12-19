#include "game/Tetromino.hpp"

const std::vector<CellColorType> Tetromino::tetromino_colors = {
    CellColorType::Red,
    CellColorType::Blue,
    CellColorType::Yellow,
    CellColorType::Green};

const std::vector<char> Tetromino::types = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};

CellColorType Tetromino::getColor() const
{
    return color;
}

const std::vector<std::vector<int>> &Tetromino::getShape() const
{
    return shape;
}
void Tetromino::evolveStates(bool forward, TetrisAction lastMove, bool gravity)
{
    int sign = (forward) ? 1 : -1;

    if (gravity)
    {
        coordinate.x += sign;
    }

    switch (lastMove)
    {

    case TetrisAction::LEFT:
        coordinate.y -= sign;
        break;
    case TetrisAction::RIGHT:
        coordinate.y += sign;
        break;
    case TetrisAction::DROP_FASTER:
        coordinate.x += sign;
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