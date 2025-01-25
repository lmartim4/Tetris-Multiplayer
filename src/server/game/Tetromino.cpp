#include "game/Tetromino.hpp"

const std::vector<CellColor> Tetromino::tetromino_colors = {
    CellColor::Red,
    CellColor::Blue,
    CellColor::Yellow,
    CellColor::Green};

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
        coordinate.setX(coordinate.getX() + sign);
        break;
    case TetrisAction::LEFT:
        coordinate.setY(coordinate.getY() - sign);
        break;
    case TetrisAction::RIGHT:
        coordinate.setY(coordinate.getY() + sign);
        break;
    case TetrisAction::DROP_FASTER:
        coordinate.setX(coordinate.getX() + sign);
        break;
    case TetrisAction::ROTATE_CCW:
        shape.rotate(!forward);
        break;
    case TetrisAction::ROTATE_CW:
        shape.rotate(forward);
        break;
    default:
        break;
    }
}