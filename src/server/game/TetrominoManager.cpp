#include "TetrominoManager.hpp"

bool TetrominoManager::spawnNextTetromino(Player *player,
                                          std::shared_ptr<BoardController> boardController,
                                          std::shared_ptr<TetrominoController> tetrominoController)
{
    if (nextTetromino.find(player) == nextTetromino.end())
        nextTetromino.emplace(player, TetrominoFactory::createTetromino());

    std::shared_ptr<Tetromino> spawningTetromino = nextTetromino.find(player)->second;

    currentTetromino.erase(player);
    currentTetromino.emplace(player, spawningTetromino);

    int maxTries = boardController->getBoard()->getWidth();

    CollisionType col;

    do
    {
        tetrominoController->transform(spawningTetromino, TetrisAction::RIGHT, true);
        col = boardController->checkCollision(spawningTetromino, TetrisAction::RIGHT, tetrominoController);

        if (maxTries == 0)
        {
            return false;
        }

    } while (col != CollisionType::NONE && maxTries-- > 0);

    if (col == CollisionType::NONE)
    {
        tetrominoController->transform(spawningTetromino, TetrisAction::RIGHT, true);
        boardController->setCellState(player->getid(), spawningTetromino, CellState::FALLING);
    }

    nextTetromino.erase(player);
    nextTetromino.emplace(player, TetrominoFactory::createTetromino());
    return true;
}