#pragma once

enum class TetrisAction
{
    EMPTY = 0,
    LEFT,
    RIGHT,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    DROP_FASTER,
    DROP_INSTANT
};

inline const char *TetrisActionToString(TetrisAction action)
{
    switch (action)
    {
    case TetrisAction::EMPTY:
        return "EMPTY";
    case TetrisAction::LEFT:
        return "LEFT";
    case TetrisAction::RIGHT:
        return "RIGHT";
    case TetrisAction::ROTATE_LEFT:
        return "ROTATE_LEFT";
    case TetrisAction::ROTATE_RIGHT:
        return "ROTATE_RIGHT";
    case TetrisAction::DROP_FASTER:
        return "DROP_FASTER";
    case TetrisAction::DROP_INSTANT:
        return "DROP_INSTANT";
    default:
        return "UNKNOWN_ACTION";
    }
}