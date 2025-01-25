#pragma once

#include "Tetromino.hpp"

class TetrominoController
{
private:
    void rotate(std::shared_ptr<Tetromino> &tetromino, TetrisAction action)
    {
        if (!(action == TetrisAction::ROTATE_CW || action == TetrisAction::ROTATE_CCW))
        {
            std::cout << "Why is it rotating with a bad action?\n";
            return;
        }

        const std::vector<std::vector<int>> shape = tetromino->getShape()->getShape();

        size_t rows = shape.size();
        size_t cols = rows > 0 ? shape[0].size() : 0;

        std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows, 0));

        if (action == TetrisAction::ROTATE_CCW)
        {
            for (size_t i = 0; i < rows; ++i)
                for (size_t j = 0; j < cols; ++j)
                    rotated[j][rows - i - 1] = shape[i][j];
        }
        else
        {
            for (size_t i = 0; i < rows; ++i)
                for (size_t j = 0; j < cols; ++j)
                    rotated[cols - j - 1][i] = shape[i][j];
        }

        tetromino->setShape(rotated);
    }

public:
    TetrominoController() = default;

    void setCanMove(std::shared_ptr<Tetromino> &tetromino, bool b)
    {
        tetromino->canMove = b;
    }
    void setLockedInPlace(std::shared_ptr<Tetromino> &tetromino, bool b)
    {
        tetromino->lockedInPlace = b;
    }

    bool canMove(const std::shared_ptr<Tetromino> &tetromino)
    {
        return tetromino->canMove;
    }

    bool isLockedInPlace(const std::shared_ptr<Tetromino> &tetromino)
    {
        return tetromino->lockedInPlace;
    }

    void transform(std::shared_ptr<Tetromino> &tetromino, TetrisAction action, bool forward)
    {
        if (action == TetrisAction::HARD_DROP)
        {
            return;
        }

        int sign = (forward) ? 1 : -1;

        if (forward == false)
            tetromino->setChanged(false);
        else
            tetromino->setChanged(true);

        switch (action)
        {
        case TetrisAction::GRAVITY:
            tetromino->getCoordinate().addX(sign);
            break;
        case TetrisAction::LEFT:
            tetromino->getCoordinate().addY(-sign);
            break;
        case TetrisAction::RIGHT:
            tetromino->getCoordinate().addY(sign);
            break;
        case TetrisAction::DROP_FASTER:
            tetromino->getCoordinate().addX(sign);
            break;
        case TetrisAction::ROTATE_CCW:
            rotate(tetromino, forward ? TetrisAction::ROTATE_CCW : TetrisAction::ROTATE_CW);
            break;
        case TetrisAction::ROTATE_CW:
            rotate(tetromino, forward ? TetrisAction::ROTATE_CW : TetrisAction::ROTATE_CCW);
            break;
        default:
            break;
        }
    }
};