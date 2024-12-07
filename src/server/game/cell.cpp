#include "game/cell.hpp"

// Construtor parametrizado
Cell::Cell(int x, int y) : x(x), y(y)
{
    setEmpty();
}

// Verifica se a célula está vazia
bool Cell::isEmpty() const
{
    return state == CELL_EMPTY;
}

// Verifica se a célula está em queda
bool Cell::isFalling() const
{
    return state == CELL_FALLING;
}

// Verifica se a célula está fixa
bool Cell::isFixed() const
{
    return state == CELL_FALLED;
}

void Cell::setEmpty()
{
    state = CELL_EMPTY;
    color = CellColorType::Empty;
}

// Define o estado da célula como em queda
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
