#pragma once

#include <string>
#include <vector>

#include "Cell.hpp"
#include "TetrominoShape.hpp"
#include "TetrisAction.hpp"
#include "Coordinate.hpp"
#include "ISerializable.hpp"

class TetrominoController;
class Tetromino : public ISerializable
{
private:
    static int instanceCount;

    Coordinate coordinate;
    int pieceId;
    bool changed = false;

    void setShape(std::vector<std::vector<int>> &s);
    void setCoordinate(Coordinate &coord);
    void setChanged(bool c) { changed = c; };

    /*Non Serializable Varaible*/
    bool canMove = false;
    bool lockedInPlace = false;

protected:
    std::shared_ptr<TetrominoShape> shape;
    CellColor color;

public:
    static const std::vector<CellColor> tetromino_colors;

    Coordinate getCoordinate() const { return coordinate; }

    Tetromino()
    {
        ++instanceCount; // Increment counter when a new object is created
        // std::cout << "Tetromino created. Instances: " << instanceCount << std::endl;
    }

    // Destructor
    ~Tetromino()
    {
        --instanceCount; // Decrement counter when an object is destroyed
        // std::cout << "Tetromino destroyed. Instances: " << instanceCount << std::endl;
    }

    Tetromino(Coordinate coord, CellColor color, const std::vector<std::vector<int>> &s) : coordinate(coord), color(color)
    {
        ++instanceCount; // Increment counter when a new object is created
        // std::cout << "Tetromino created. Instances: " << instanceCount << std::endl;
        
        shape = std::make_shared<TetrominoShape>(s);

        static int nextId = 1;
        pieceId = nextId++;

        changed = true;
    }

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

    // Serialization
    nlohmann::json serialize() const override
    {
        return nlohmann::json{
            {"coordinate", coordinate.serialize()},
            {"pieceId", pieceId},
            {"shape", shape->serialize()},
            {"changed", true},
            {"color", static_cast<int>(color)}};
    }

    void deserialize(const nlohmann::json &data) override
    {
        if (data.contains("coordinate"))
            coordinate.deserialize(data["coordinate"]);
        else
            throw std::runtime_error("Missing coordinate in Tetromino deserialization");

        if (data.contains("pieceId") && data["pieceId"].is_number_integer())
            pieceId = data["pieceId"];
        else
            throw std::runtime_error("Missing or invalid pieceId in Tetromino deserialization");

        if (data.contains("changed") && data["changed"].is_boolean())
            changed = data["changed"];
        else
            throw std::runtime_error("Missing or invalid changed in Tetromino deserialization");

        if (data.contains("shape"))
        {
            shape = std::make_shared<TetrominoShape>();
            shape->deserialize(data["shape"]);
        }
        else
            throw std::runtime_error("Missing shape in Tetromino deserialization");

        if (data.contains("color") && data["color"].is_number_integer())
            color = static_cast<CellColor>(data["color"]);
        else
            throw std::runtime_error("Missing or invalid color in Tetromino deserialization");
    }

    friend class TetrominoController;
};