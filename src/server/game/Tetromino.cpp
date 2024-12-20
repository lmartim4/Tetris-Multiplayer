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

    if (!forward)
        hasChanged = false;

    if (gravity)
    {
        coordinate.x += sign;
        hasChanged = true;
    }

    switch (lastMove)
    {
    case TetrisAction::LEFT:
        coordinate.y -= sign;
        hasChanged = true;
        break;
    case TetrisAction::RIGHT:
        coordinate.y += sign;
        hasChanged = true;
        break;
    case TetrisAction::DROP_FASTER:
        coordinate.x += sign;
        hasChanged = true;
        break;
    case TetrisAction::ROTATE_LEFT:
        rotate(forward ? false : true);
        hasChanged = true;
        break;
    case TetrisAction::ROTATE_RIGHT:
        rotate(forward ? true : false);
        hasChanged = true;
        break;
    default:
        break;
    }
}