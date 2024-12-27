#include "Cell.hpp"
#include <iostream>

Cell::Cell(Coordinate coord) : coordinate(coord)
{
    setEmpty();
}

void Cell::setEmpty()
{
    state = EMPTY;
    color = CellColor::Empty;
}

void Cell::setState(CellState s)
{
    state = s;
}

CellColor Cell::getColor() const
{
    return color;
};

CellState Cell::getState() const
{
    return state;
}

void Cell::setColor(CellColor c)
{
    color = c;
}
