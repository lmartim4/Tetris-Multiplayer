#pragma once

#include <memory>
#include "baseTetromino.hpp"
#include <iostream>
#include "game/cell.hpp"
#include "json.hpp"

class ServerManager;

class TetrisBoard
{
private:
    std::vector<std::vector<std::shared_ptr<Cell>>> grid; // Matriz 2D para representar a grid e seus estados/logica
    ServerManager &serverManager;

public:
    // In terms of the number of cells
    static const int WIDTH;
    static const int HEIGHT;

    TetrisBoard(ServerManager &serverManager);

    bool reachedTop();
    void printStatus();
    void clear();

    void handleInput(Tetromino &currentTetromino, TetrisAction action);

    // Melhor seria se fosse const, mas é a vida, não consigo pensar em outro jeito de
    // checar a colisao sem mudar o objeto e ver se dá merda, na prática faz-se a ação inversa depois, então safe
    bool checkCollision(Tetromino &currentTetromino);
    int clearLines();
    bool placeTetromino(const Tetromino &currentTetromino, bool fallen);
    static int normalizedY(int y);
    void clearFallingTetrominos();
    void clearFalledTetrominos();
    std::vector<std::vector<std::shared_ptr<Cell>>> &getGrid();

    void broadcastBoardState();
};