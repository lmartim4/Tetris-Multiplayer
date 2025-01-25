#include "game/Tetromino.hpp"
#include "Tetromino.hpp"

const std::vector<CellColor> Tetromino::tetromino_colors = {
    CellColor::Red,
    CellColor::Blue,
    CellColor::Yellow,
    CellColor::Green};

CellColor Tetromino::getColor() const
{
    return color;
}

std::shared_ptr<TetrominoShape> Tetromino::getShape() const
{
    return shape;
}

void Tetromino::setShape(std::vector<std::vector<int>> &s)
{
    shape->setShape(s);
    changed = true;
}

void Tetromino::setCoordinate(Coordinate &coord)
{
    coordinate = coord;
}
