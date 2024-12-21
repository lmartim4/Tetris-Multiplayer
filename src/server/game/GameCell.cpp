#include "GameCell.hpp"

GameCell::GameCell(int x, int y) : x(x), y(y) { setEmpty(); }

void GameCell::setEmpty()
{
    state = CELL_EMPTY;
    color = CellColorType::Empty;
}

void GameCell::setFalling(const CellColorType &newColor)
{
    state = CELL_FALLING;
    color = newColor;
}

void GameCell::setFixed(const CellColorType &newColor)
{
    state = CELL_FALLED;
    color = newColor;
}
