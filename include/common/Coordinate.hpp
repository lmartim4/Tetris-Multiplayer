#pragma once

#include "ISerializable.hpp"

struct Coordinate : public ISerializable
{
private:
    int x, y;

public:
    Coordinate() {};
    Coordinate(int xVal, int yVal) : x(xVal), y(yVal) {}

    int getX() { return x; }
    int getY() { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }

    nlohmann::json serialize() const override
    {
        return nlohmann::json{{"x", x}, {"y", y}};
    }

    void deserialize(const nlohmann::json &json) override
    {
        this->x = json.at("x").get<int>();
        this->y = json.at("y").get<int>();
    }
};