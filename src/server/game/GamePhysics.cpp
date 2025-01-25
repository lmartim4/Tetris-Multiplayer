#include "GamePhysics.hpp"

int GamePhysics::countNewLockedTetrominos(std::vector<std::shared_ptr<Tetromino>> tetrominos,
                                          std::shared_ptr<BoardController> boardController,
                                          std::shared_ptr<TetrominoController> tetrominoController)
{
    int count = 0;

    for (const std::shared_ptr<Tetromino> tetromino : tetrominos)
    {
        CollisionType col = boardController->checkCollision(tetromino, TetrisAction::GRAVITY, tetrominoController);

        if (col == CollisionType::FALLEN_FIXED || col == CollisionType::GROUND)
            if (!tetrominoController->isLockedInPlace(tetromino))
                count++;
    }

    return count;
}

CollisionType GamePhysics::applyAction(std::shared_ptr<Tetromino> tetromino,
                                       TetrisAction action,
                                       std::shared_ptr<BoardController> boardController,
                                       std::shared_ptr<TetrominoController> tetrominoController,
                                       std::shared_ptr<TetrominoManager> tetrominoManager)
{
    boardController->clearFallingTetromino(tetromino);

    CollisionType colision = boardController->checkCollision(tetromino, action, tetrominoController);

    if (colision != CollisionType::NONE)
    {
        // server.broadcastSound(SoundType::DenyErrorSound);
    }
    else
    {
        // server.broadcastSound(SoundType::FabricImpactSound);
        tetrominoController->transform(tetromino, action, true);
    }

    boardController->setCellState(tetromino, CellState::FALLING);

    if (colision != CollisionType::NONE)
        onTetrominoColide(tetromino, colision, action, boardController, tetrominoController, tetrominoManager);

    return colision;
}

CollisionType GamePhysics::applyHardDrop(std::shared_ptr<Tetromino> tetromino,
                                         std::shared_ptr<BoardController> boardController,
                                         std::shared_ptr<TetrominoController> tetrominoController,
                                         std::shared_ptr<TetrominoManager> tetrominoManager)
{
    boardController->clearFallingTetromino(tetromino);
    CollisionType colision;
    do
    {
        colision = boardController->checkCollision(tetromino, TetrisAction::GRAVITY, tetrominoController);

        if (colision == CollisionType::NONE)
            tetrominoController->transform(tetromino, TetrisAction::GRAVITY, true);

    } while (colision == CollisionType::NONE);

    boardController->setCellState(tetromino, CellState::FALLING);
    onTetrominoColide(tetromino, colision, TetrisAction::HARD_DROP, boardController, tetrominoController, tetrominoManager);
    return colision;
}

void GamePhysics::onTetrominoColide(std::shared_ptr<Tetromino> tetromino,
                                    CollisionType col,
                                    TetrisAction action,
                                    std::shared_ptr<BoardController> boardController,
                                    std::shared_ptr<TetrominoController> tetrominoController,
                                    std::shared_ptr<TetrominoManager> tetrominoManager)
{
    if (col == CollisionType::FALLING_OTHER)
        return;

    if (!(action == TetrisAction::GRAVITY || action == TetrisAction::DROP_FASTER || action == TetrisAction::HARD_DROP))
        return;

    boardController->setCellState(tetromino, CellState::FALLEN);
    tetrominoController->setLockedInPlace(tetromino, true);
}

void GamePhysics::applyGravity(std::shared_ptr<BoardController> boardController,
                               std::shared_ptr<TetrominoController> tetrominoController,
                               std::shared_ptr<TetrominoManager> tetrominoManager)
{
    if (!gravity.hasGravityIntervalElapsed())
        return;

    CollisionType colision;

    std::vector<std::shared_ptr<Tetromino>> tetrominosToApplyGravity;

    for (std::shared_ptr<Tetromino> tetromino : tetrominoManager->getCurrentTetrominos())
    {
        tetrominosToApplyGravity.emplace_back(tetromino);
        tetrominoController->setCanMove(tetromino, false);
        tetrominoController->setLockedInPlace(tetromino, false);
    }

    while (countNewLockedTetrominos(tetrominosToApplyGravity, boardController, tetrominoController) > 0)
    {
        for (std::shared_ptr<Tetromino> tetromino : tetrominosToApplyGravity)
        {
            if (tetrominoController->isLockedInPlace(tetromino))
                continue;

            colision = boardController->checkCollision(tetromino, TetrisAction::GRAVITY, tetrominoController);

            if (colision == CollisionType::GROUND || colision == CollisionType::FALLEN_FIXED)
            {
                tetrominoController->setCanMove(tetromino, false);
                boardController->setCellState(tetromino, CellState::FALLING);
                onTetrominoColide(tetromino, colision, TetrisAction::GRAVITY, boardController, tetrominoController, tetrominoManager);
            }
            else if (colision == CollisionType::FALLING_OTHER || colision == CollisionType::NONE)
            {
                tetrominoController->setCanMove(tetromino, true);
            }
        }
    }

    for (std::shared_ptr<Tetromino> tetromino : tetrominosToApplyGravity)
    {
        colision = boardController->checkCollision(tetromino, TetrisAction::GRAVITY, tetrominoController);

        if (colision == CollisionType::FALLING_OTHER || colision == CollisionType::NONE)
        {
            boardController->clearFallingTetromino(tetromino);
            tetrominoController->transform(tetromino, TetrisAction::GRAVITY, true);
            boardController->setCellState(tetromino, CellState::FALLING);
        }
        // if (colision != CollisionType::NONE)  onTetrominoColide(tetromino, colision, TetrisAction::GRAVITY, boardController, tetrominoController, tetrominoManager);
    }
}