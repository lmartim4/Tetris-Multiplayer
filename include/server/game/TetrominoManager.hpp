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
                            std::shared_ptr<TetrominoController> tetrominoController, int maxTries);

    std::shared_ptr<Tetromino> getCurrentTetromino(Player *player) const
    {
        auto it = currentTetromino.find(player);
        if (it != currentTetromino.end())
            return it->second;
        return nullptr;
    }

    std::shared_ptr<Tetromino> getNextTetromino(Player *player) const
    {
        auto it = nextTetromino.find(player);
        if (it != nextTetromino.end())
            return it->second;
        return nullptr;
    }

    std::vector<std::shared_ptr<Tetromino>> getCurrentTetrominos() const
    {
        std::vector<std::shared_ptr<Tetromino>> tetrominos;

        for (const auto &pair : currentTetromino)
        {
            if (auto tetromino = pair.second)
            {
                tetrominos.push_back(tetromino);
            }
        }

        return tetrominos;
    }

    Player *getPlayerByTetromino(std::shared_ptr<Tetromino> tetromino) const
    {
        for (const auto &pair : currentTetromino)
        {
            if (auto currentTetromino = pair.second)
            {
                if (currentTetromino == tetromino)
                {
                    return pair.first;
                }
            }
        }

        return nullptr;
    }

    TetrominoManager() = default;
    ~TetrominoManager() { std::cout << "Deleting TetrominoManager\n"; };
};
