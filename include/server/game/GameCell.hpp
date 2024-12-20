#pragma once

#include "CellColorType.hpp"

enum CELL_STATE
{
    CELL_FALLED = -1,
    CELL_EMPTY = 0,
    CELL_FALLING = 1
};

class GameCell
{
private:
    CELL_STATE state; // 0 = vazio, -1 = fixado, 1 = em queda
    CellColorType color;

    int x;
    int y;
public:
    GameCell(int x, int y);

    CellColorType getColor() const { return color; };

    bool isEmpty() const;
    bool isFalling() const;
    bool isFixed() const;

    void setEmpty();
    void setFalling(const CellColorType &newColor);
    void setFixed(const CellColorType &newColor);

    bool operator==(const GameCell &other) const
    {
        return color == other.color;
    }

    bool operator!=(const GameCell &other) const
    {
        return !(*this == other);
    }
};