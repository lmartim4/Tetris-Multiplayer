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
    int ownerID = -1;

public:
    Cell(Coordinate coord);
    ~Cell() override {}

    int getOwnerID() const;
    int getPieceId() const;
    void setPieceId(int id);

    CellColor getColor() const;
    CellState getState() const;

    void setColor(CellColor c);
    void setState(CellState s);
    void setOwnerID(int o);

    void setEmpty();

    nlohmann::json serialize() const override;
    void deserialize(const nlohmann::json &json);
};
