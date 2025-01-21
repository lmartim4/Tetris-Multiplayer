#pragma once

#include "Coordinate.hpp"
#include "CellColor.hpp"

class Cell
{
private:
    CellState state;
    CellColor color;
    Coordinate coordinate;

    int pieceId = 0;

public:
    Cell(Coordinate coord);
    ~Cell() {}

    int getPieceId() const;
    void setPieceId(int id);

    CellColor getColor() const;
    CellState getState() const;

    void setColor(CellColor c);
    void setState(CellState s);

    void setEmpty();
};