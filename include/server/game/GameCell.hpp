#pragma once

#include "CellColorType.hpp"

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
    CellColorType color;
    
    int x;
    int y;

public:
    GameCell(int x, int y);

    CellColorType getColor() const { return color; };
    CELL_STATE getState() const { return state; }

    void setEmpty();
    void setFalling(const CellColorType &newColor);
    void setFixed(const CellColorType &newColor);
};