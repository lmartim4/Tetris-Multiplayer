#include "TetrominoManager.hpp"

bool TetrominoManager::spawnNextTetromino(Player *player,
                                          std::shared_ptr<BoardController> boardController,
                                          std::shared_ptr<TetrominoController> tetrominoController,
                                          int maxTries)
{
    // Ensure the player has a "next tetromino" to spawn
    if (nextTetromino.find(player) == nextTetromino.end())
    {
        nextTetromino[player] = TetrominoFactory::createTetromino();
    }

    // Convert the weak_ptr to shared_ptr
    std::shared_ptr<Tetromino> spawningTetromino = nextTetromino[player];
    if (!spawningTetromino)
    {
        throw std::logic_error("Failed to lock the next tetromino. It might have been deleted.");
    }

    // Move the tetromino from "next" to "current"
    currentTetromino[player] = spawningTetromino;

    CollisionType collisionStatus;

    // Try placing the tetromino up to `maxTries` times
    do
    {
        tetrominoController->transform(spawningTetromino, TetrisAction::RIGHT, true);
        collisionStatus = boardController->checkCollision(spawningTetromino, TetrisAction::RIGHT, tetrominoController);

        if (maxTries <= 0)
        {
            currentTetromino.clear();
            nextTetromino.clear();
            return false;
        }

        --maxTries;

    } while (collisionStatus != CollisionType::NONE);

    // Place the tetromino on the board if no collision occurs
    if (collisionStatus == CollisionType::NONE)
    {
        tetrominoController->transform(spawningTetromino, TetrisAction::RIGHT, true);
        boardController->setCellState(player->getid(), spawningTetromino, CellState::FALLING);
    }

    // Generate the next tetromino for the player
    nextTetromino[player] = TetrominoFactory::createTetromino();

    return true;
}
