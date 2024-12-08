#include "game/Cell.hpp"

Cell::Cell(int x, int y) : x(x), y(y) { setEmpty(); }

bool Cell::isEmpty() const { return state == CELL_EMPTY; }
bool Cell::isFalling() const { return state == CELL_FALLING; }

bool Cell::isFixed() const { return state == CELL_FALLED; }

void Cell::setEmpty()
{
    state = CELL_EMPTY;
    color = CellColorType::Empty;
}

void Cell::setFalling(const CellColorType &newColor)
{
    state = CELL_FALLING;
    color = newColor;
}

void Cell::setFixed(const CellColorType &newColor)
{
    state = CELL_FALLED;
    color = newColor;
}
