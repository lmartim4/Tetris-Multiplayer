#include "game/Tetromino.hpp"

const std::vector<CellColor> Tetromino::tetromino_colors = {
    CellColor::Red,
    CellColor::Blue,
    CellColor::Yellow,
    CellColor::Green};

const std::vector<char> Tetromino::types = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};

CellColor Tetromino::getColor() const
{
    return color;
}

const TetrominoShape Tetromino::getShape() const
{
    return shape;
}
void Tetromino::evolveStates(bool forward, TetrisAction lastMove)
{
    int sign = (forward) ? 1 : -1;

    if (forward == false)
        hasChanged = false;
    else
        hasChanged = true;

    switch (lastMove)
    {
    case TetrisAction::GRAVITY:
        coordinate.x += sign;
        break;

    case TetrisAction::LEFT:
        coordinate.y -= sign;
        break;
    case TetrisAction::RIGHT:
        coordinate.y += sign;
        break;
    case TetrisAction::DROP_FASTER:
        coordinate.x += sign;
        break;
    case TetrisAction::ROTATE_CCW:
        shape.rotate(forward ? false : true);
        break;
    case TetrisAction::ROTATE_CW:
        shape.rotate(forward ? true : false);
        break;
    default:
        break;
    }
}