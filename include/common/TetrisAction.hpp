#pragma once
#include "PacketType.hpp"

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

inline const TetrisAction getActionFromPacketType(PacketType type)
{
    TetrisAction action = TetrisAction::EMPTY;
    switch (type)
    {
    case PacketType::LEFT:
        action = TetrisAction::LEFT;
        break;
    case PacketType::RIGHT:
        action = TetrisAction::RIGHT;
        break;
    case PacketType::ROTATE_LEFT:
        action = TetrisAction::ROTATE_LEFT;
        break;
    case PacketType::ROTATE_RIGHT:
        action = TetrisAction::ROTATE_RIGHT;
        break;
    case PacketType::DROP_FASTER:
        action = TetrisAction::DROP_FASTER;
        break;
    case PacketType::DROP_INSTANT:
        action = TetrisAction::DROP_INSTANT;
        break;
    default:
        break;
    }

    return action;
}
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