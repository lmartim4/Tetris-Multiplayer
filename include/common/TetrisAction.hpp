#pragma once

#include <string>
#include "Packet.hpp"

enum class TetrisAction
{
    LEFT,
    RIGHT,
    ROTATE_CCW,
    ROTATE_CW,
    DROP_FASTER,
    HARD_DROP,
    GRAVITY,
};

inline const TetrisAction getActionFromPacketType(PacketType type)
{
    TetrisAction action = TetrisAction::GRAVITY;
    switch (type)
    {
    case PacketType::LEFT:
        action = TetrisAction::LEFT;
        break;
    case PacketType::RIGHT:
        action = TetrisAction::RIGHT;
        break;
    case PacketType::ROTATE_CCW:
        action = TetrisAction::ROTATE_CCW;
        break;
    case PacketType::ROTATE_CW:
        action = TetrisAction::ROTATE_CW;
        break;
    case PacketType::DROP_FASTER:
        action = TetrisAction::DROP_FASTER;
        break;
    case PacketType::HARD_DROP:
        action = TetrisAction::HARD_DROP;
        break;
    default:
        break;
    }

    return action;
}

inline std::string TetrisActionToString(TetrisAction action)
{
    switch (action)
    {
    case TetrisAction::LEFT:
        return "LEFT";
    case TetrisAction::RIGHT:
        return "RIGHT";
    case TetrisAction::ROTATE_CCW:
        return "ROTATE_CCW";
    case TetrisAction::ROTATE_CW:
        return "ROTATE_CW";
    case TetrisAction::DROP_FASTER:
        return "DROP_FASTER";
    case TetrisAction::HARD_DROP:
        return "HARD_DROP";
    default:
        return "UNKNOWN_ACTION";
    }
}