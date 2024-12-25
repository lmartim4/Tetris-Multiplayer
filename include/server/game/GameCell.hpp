#pragma once

#include "CellColor.hpp"

enum CELL_STATE
{
    CELL_FALLEN,
    CELL_EMPTY,
    CELL_FALLING
};

class GameCell
{
private:
    CELL_STATE state;
    CellColor color;
    
    int x;
    int y;

public:
    GameCell(int x, int y);

    CellColor getColor() const { return color; };
    CELL_STATE getState() const { return state; }

    void setEmpty();
    void setFalling(const CellColor &newColor);
    void setFixed(const CellColor &newColor);
};