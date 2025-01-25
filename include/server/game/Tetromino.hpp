#pragma once

#include <string>
#include <vector>

#include "Cell.hpp"
#include "TetrominoShape.hpp"
#include "TetrisAction.hpp"
#include "Coordinate.hpp"

class TetrominoController;
class Tetromino
{
private:
    Coordinate coordinate;

    int pieceId;
    bool changed = true;

    /*ControlVariables for gravity processing*/
    bool canMove = false;
    bool lockedInPlace = false;
    
    void setShape(std::vector<std::vector<int>> &s);
    void setCoordinate(Coordinate &coord);
    void setChanged(bool c) { changed = c; };

protected:
    std::shared_ptr<TetrominoShape> shape;
    CellColor color;

public:
    static const std::vector<CellColor> tetromino_colors;

    Coordinate getCoordinate() const { return coordinate; }

    Tetromino(Coordinate coord, CellColor color, const std::vector<std::vector<int>> &s) : coordinate(coord), color(color)
    {
        shape = std::make_shared<TetrominoShape>(s);

        static int nextId = 1;
        pieceId = nextId++;

        changed = true;
    }

    virtual ~Tetromino() = default;

    CellColor getColor() const;

    std::shared_ptr<TetrominoShape> getShape() const;

    Coordinate &getCoordinate() { return coordinate; };

    int getId() const { return pieceId; }

    bool hasChanged() { return changed; }

    bool shouldBroadcastState()
    {
        if (!hasChanged())
            return false;

        setChanged(false);
        return true;
    }

    friend class TetrominoController;
};