#pragma once

#include <memory>
#include <iostream>
#include "json.hpp"
#include "Tetromino.hpp"
#include "game/Cell.hpp"

class ServerManager;

class TetrisBoard
{
private:
    std::vector<std::vector<std::shared_ptr<Cell>>> grid;
    std::vector<std::vector<std::shared_ptr<Cell>>> lastBroadcastedGrid;

public:
    static const int WIDTH;
    static const int HEIGHT;
    bool changed = false;
    bool anyChanges() { return changed; }

    TetrisBoard();

    bool reachedTop();
    void printStatus();
    void clear();

    // Melhor seria se fosse const, mas é a vida, não consigo pensar em outro jeito de
    // checar a colisao sem mudar o objeto e ver se dá merda, na prática faz-se a ação inversa depois, então safe
    bool checkCollision(Tetromino &currentTetromino, TetrisAction action);
    int clearLines();

    bool placeTetromino(const Tetromino &currentTetromino, bool fallen);

    static int normalizedY(int y);

    void clearFallingTetrominos();
    void clearFalledTetrominos();

    std::vector<std::vector<std::shared_ptr<Cell>>> &getGrid();

    // bool gridsAreEqual(const std::vector<std::vector<std::shared_ptr<Cell>>> &grid1, const std::vector<std::vector<std::shared_ptr<Cell>>> &grid2) const;

    nlohmann::json constructBoardJsonToBroadcast();
};