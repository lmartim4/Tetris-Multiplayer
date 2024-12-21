#pragma once

#include <memory>
#include <iostream>
#include "Debuggable.hpp"
#include "json.hpp"
#include "Tetromino.hpp"
#include "GameCell.hpp"

class ServerManager;

class TetrisBoard : public Debuggable
{
private:
    int WIDTH;
    int HEIGHT;

    std::vector<std::vector<std::shared_ptr<GameCell>>> grid;
    std::vector<std::vector<std::shared_ptr<GameCell>>> lastBroadcastedGrid;

    int getNormalizedY(int y) const;

public:
    TetrisBoard(int h, int w);

    bool reachedTop() const;
    void printStatus() const;
    void clear() const;

    bool tryMove(Tetromino &currentTetromino, TetrisAction action) const;
    int clearLines() const;
    void placeTetromino(const Tetromino &currentTetromino, bool fallen);
    void clearFallingTetrominos();
    void clearFalledTetrominos();

    std::vector<std::vector<std::shared_ptr<GameCell>>> &getGrid() { return grid; }
    nlohmann::json constructBoardJsonToBroadcast() const;
};