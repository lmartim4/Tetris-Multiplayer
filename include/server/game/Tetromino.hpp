#pragma once

#include <string>
#include <vector>

#include "Cell.hpp"
#include "TetrominoShape.hpp"
#include "TetrisAction.hpp"
#include "Coordinate.hpp"

class Tetromino
{
private:
    Coordinate coordinate;

    int pieceId;
    bool hasChanged;

protected:
    TetrominoShape shape;
    CellColor color;

public:
    static const std::vector<CellColor> tetromino_colors;

    bool canMove = false;
    bool lockedInPlace = false;

    Coordinate getCoordinate() const { return coordinate; }

    Tetromino(Coordinate coord, CellColor color, const std::vector<std::vector<int>> &shape) : coordinate(coord), color(color), shape(shape)
    {
        static int nextId = 1;
        pieceId = nextId++;

        hasChanged = true;
    }
    virtual ~Tetromino() = default;

    CellColor getColor() const;

    const TetrominoShape getShape() const;

    int getId() const { return pieceId; }

    bool shouldBroadcastState()
    {
        if (hasChanged)
        {
            hasChanged = false;
            return true;
        }

        return false;
    }

    void evolveStates(bool forward, TetrisAction lastMove);
};
