#pragma once

#include <vector>
#include "ISerializable.hpp"

class TetrominoShape : public ISerializable
{
private:
    // 2D grid representing the shape of the Tetromino
    std::vector<std::vector<int>> shape;

public:
    // Default constructor
    TetrominoShape() = default;

    // Constructor with shape initialization
    TetrominoShape(const std::vector<std::vector<int>> &shape) : shape(shape) {}

    // Getters and setters
    const std::vector<std::vector<int>> &getShape() const { return shape; }
    void setShape(const std::vector<std::vector<int>> &newShape) { shape = newShape; }

    // ISerializable implementation
    nlohmann::json serialize() const override
    {
        return nlohmann::json{{"shape", shape}};
    }

    void deserialize(const nlohmann::json &data) override
    {
        if (data.contains("shape") && data["shape"].is_array())
        {
            shape = data["shape"].get<std::vector<std::vector<int>>>();
        }
        else
        {
            throw std::runtime_error("Invalid shape data during deserialization");
        }
    }

    // Rotate the shape (clockwise or counterclockwise)
    void rotate(bool clockwise)
    {
        size_t rows = shape.size();
        size_t cols = rows > 0 ? shape[0].size() : 0;

        std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows, 0));

        if (clockwise)
        {
            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < cols; ++j)
                {
                    rotated[j][rows - i - 1] = shape[i][j];
                }
            }
        }
        else
        {
            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < cols; ++j)
                {
                    rotated[cols - j - 1][i] = shape[i][j];
                }
            }
        }

        shape = rotated;
    }

    void invertShape()
    {
        for (auto &y : shape)
            std::reverse(y.begin(), y.end());
    }
};
