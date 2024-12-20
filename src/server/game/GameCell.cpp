#include "GameCell.hpp"

GameCell::GameCell(int x, int y) : x(x), y(y) { setEmpty(); }

bool GameCell::isEmpty() const { return state == CELL_EMPTY; }
bool GameCell::isFalling() const { return state == CELL_FALLING; }

bool GameCell::isFixed() const { return state == CELL_FALLED; }

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
