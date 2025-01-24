#include "Cell.hpp"
#include <iostream>

Cell::Cell(Coordinate coord) : coordinate(coord)
{
    setEmpty();
}

void Cell::setEmpty()
{
    state = EMPTY;
    color = CellColor::Empty;
}

void Cell::setState(CellState s)
{
    state = s;
}

int Cell::getPieceId() const
{
    return pieceId;
}
void Cell::setPieceId(int id)
{
    pieceId = id;
}

CellColor Cell::getColor() const
{
    return color;
}

CellState Cell::getState() const
{
    return state;
}

void Cell::setColor(CellColor c)
{
    color = c;
}

nlohmann::json Cell::serialize() const
{
    return nlohmann::json{
        {"state", state},
        {"color", color},
        {"coordinate", coordinate.serialize()},
        {"pieceId", pieceId}};
}

void Cell::deserialize(const nlohmann::json &json)
{
    coordinate.deserialize(json.at("coordinate"));
    setState(static_cast<CellState>(json.at("state").get<int>()));
    setColor(static_cast<CellColor>(json.at("color").get<int>()));
    setPieceId(json.at("pieceId").get<int>());
}