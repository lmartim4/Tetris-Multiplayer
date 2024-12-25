#include "GameCell.hpp"

GameCell::GameCell(int x, int y) : x(x), y(y) { setEmpty(); }

void GameCell::setEmpty()
{
    state = CELL_EMPTY;
    color = CellColor::Empty;
}

void GameCell::setFalling(const CellColor &newColor)
{
    state = CELL_FALLING;
    color = newColor;
}

void GameCell::setFixed(const CellColor &newColor)
{
    state = CELL_FALLEN;
    color = newColor;
}
