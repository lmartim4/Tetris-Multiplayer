#pragma once

#include <vector>
#include "ISerializable.hpp"

class TetrominoShape : public ISerializable
{
private:
    std::vector<std::vector<int>> shape;

public:
    TetrominoShape() = default;

    TetrominoShape(const std::vector<std::vector<int>> &shape) : shape(shape) {}

    const std::vector<std::vector<int>> &getShape() const { return shape; }
    void setShape(const std::vector<std::vector<int>> &newShape) { shape = newShape; }

    nlohmann::json serialize() const override
    {
        return nlohmann::json{{"shape", shape}};
    }

    void deserialize(const nlohmann::json &data) override
    {
        if (data.contains("shape") && data["shape"].is_array())
            shape = data["shape"].get<std::vector<std::vector<int>>>();
        else
            throw std::runtime_error("Invalid shape data during deserialization");
    }

    void invertShape()
    {
        for (auto &y : shape)
            std::reverse(y.begin(), y.end());
    }
};
