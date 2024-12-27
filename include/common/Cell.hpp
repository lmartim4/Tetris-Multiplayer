#pragma once

#include "Coordinate.hpp"
#include "CellColor.hpp"

class Cell
{
private:
    CellState state;
    CellColor color;
    Coordinate coordinate;

public:
    Cell(Coordinate coord);

    CellColor getColor() const;
    CellState getState() const;
    void setColor(CellColor c);

    void setState(CellState s);
    void setEmpty();
};