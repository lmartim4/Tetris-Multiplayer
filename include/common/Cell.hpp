#pragma once

#include "Coordinate.hpp"
#include "ISerializable.hpp"

enum CellState
{
    FALLEN,
    EMPTY,
    FALLING
};

enum CellColor
{
    Empty,
    Red,
    Green,
    Blue,
    Yellow,
};

class Cell : public ISerializable
{
private:
    CellState state;
    CellColor color;
    Coordinate coordinate;
    
    int pieceId = 0;

public:
    Cell(Coordinate coord);
    ~Cell() override {}

    int getPieceId() const;
    void setPieceId(int id);

    CellColor getColor() const;
    CellState getState() const;

    void setColor(CellColor c);
    void setState(CellState s);

    void setEmpty();

    nlohmann::json serialize() const override;
    void deserialize(const nlohmann::json &json);
};
