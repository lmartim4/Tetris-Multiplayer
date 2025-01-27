#pragma once

#include <memory>
#include <vector>
#include "Cell.hpp"
#include "ISerializable.hpp"
#include "Logger.hpp"
#include "json.hpp"

class TetrisBoard : public ISerializable
{
private:
    Logger *logger;

    int width;
    int height;

    std::vector<std::vector<std::shared_ptr<Cell>>> grid;
    void setupGrid();

public:
    TetrisBoard(int h, int w);

    ~TetrisBoard()
    {
        delete logger;
        std::cout << "Deleting TetrominoManager\n";
    };

    void printDebug() const;

    int getWidth() const
    {
        return width;
    }
    int getHeight() const
    {
        return height;
    }

    std::vector<std::vector<std::shared_ptr<Cell>>> &getGrid()
    {
        return grid;
    }

    void clearBoard() const;

    void setSize(int x, int y);

    int getNormalizedY(int y) const;

    nlohmann::json serialize() const override;
    void deserialize(const nlohmann::json &data) override;
};
