#pragma once

#include "CellColorType.hpp"

enum CELL_STATES
{
    CELL_FALLED = -1,
    CELL_EMPTY = 0,
    CELL_FALLING = 1
};

class Cell
{
private:
    int state; // 0 = vazio, -1 = fixado, 1 = em queda
    CellColorType color;

    int x; // Linha (posição X)
    int y; // Coluna (posição Y)
public:
    Cell(int x, int y);

    // Setter para a cor
    //void setColor(const CellColorType &newColor);
    CellColorType getColor() const {return color;};
    // Verificadores de estado
    bool isEmpty() const;
    bool isFalling() const;
    bool isFixed() const;

    // Setters de estado
    void setEmpty();
    void setFalling(const CellColorType &newColor);
    void setFixed(const CellColorType &newColor);
};