#pragma once

#include "BoardController.hpp"
#include "TetrominoFactory.hpp"
#include "TetrominoController.hpp"
#include "Tetromino.hpp"
#include "Player.hpp"

class TetrominoManager
{
private:
    std::unordered_map<Player *, std::shared_ptr<Tetromino>> currentTetromino;
    std::unordered_map<Player *, std::shared_ptr<Tetromino>> nextTetromino;

public:
    bool spawnNextTetromino(Player *player,
                            std::shared_ptr<BoardController> boardManager,
                            std::shared_ptr<TetrominoController> tetrominoController);

    std::shared_ptr<Tetromino> getCurrentTetromino(Player *player) const
    {
        auto it = currentTetromino.find(player);
        if (it != currentTetromino.end())
            return it->second;
        return nullptr;
    }

    std::vector<std::shared_ptr<Tetromino>> getCurrentTetrominos() const
    {
        std::vector<std::shared_ptr<Tetromino>> tetrominos;

        for (const auto &pair : currentTetromino)
            tetrominos.push_back(pair.second);

        return tetrominos;
    }

    Player *getPlayerByTetromino(std::shared_ptr<Tetromino> tetromino) const
    {
        for (const auto &pair : currentTetromino)
            if (pair.second == tetromino)
                return pair.first;
                
        return nullptr;
    }

    TetrominoManager() = default;
    ~TetrominoManager() = default;
};
