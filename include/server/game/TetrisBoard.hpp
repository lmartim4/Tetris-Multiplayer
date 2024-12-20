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

    bool reachedTop();
    void printStatus();
    void clear();

    // Melhor seria se fosse const, mas é a vida, não consigo pensar em outro jeito de
    // checar a colisao sem mudar o objeto e ver se dá merda, na prática faz-se a ação inversa depois, então safe
    bool checkCollision(Tetromino &currentTetromino, TetrisAction action, bool gravity);
    int clearLines();

    bool placeTetromino(const Tetromino &currentTetromino, bool fallen);

    void clearFallingTetrominos();
    void clearFalledTetrominos();

    std::vector<std::vector<std::shared_ptr<GameCell>>> &getGrid() { return grid; }

    nlohmann::json constructBoardJsonToBroadcast();
    // bool gridsAreEqual(const std::vector<std::vector<std::shared_ptr<GameCell>>> &grid1, const std::vector<std::vector<std::shared_ptr<GameCell>>> &grid2) const;
};