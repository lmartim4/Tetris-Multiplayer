#pragma once

struct Coordinate
{
    int x, y;
    Coordinate() = default;
    Coordinate(int xVal, int yVal) : x(xVal), y(yVal) {}
};