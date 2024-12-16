#pragma once

#include "CellColorType.hpp"

enum CELL_STATE
{
    CELL_FALLED = -1,
    CELL_EMPTY = 0,
    CELL_FALLING = 1
};

class Cell
{
private:
    CELL_STATE state; // 0 = vazio, -1 = fixado, 1 = em queda
    CellColorType color;

    int x;
    int y;
public:
    Cell(int x, int y);

    CellColorType getColor() const { return color; };

    bool isEmpty() const;
    bool isFalling() const;
    bool isFixed() const;

    void setEmpty();
    void setFalling(const CellColorType &newColor);
    void setFixed(const CellColorType &newColor);

    bool operator==(const Cell &other) const
    {
        return color == other.color;
    }

    bool operator!=(const Cell &other) const
    {
        return !(*this == other);
    }
};