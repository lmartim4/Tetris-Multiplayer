#pragma once

enum class PacketType : char
{
    // Game actions/commands Packets
    JOIN_REQUEST,
    JOIN_ACCEPTED,
    JOIN_DENIED,
    REQUEST_START,
    STARTING_GAME,
    RESTART,
    PAUSE,
    RESUME,
    LEFT,
    RIGHT,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    DROP_FASTER,
    DROP_INSTANT,

    // Network/connection-related
    HEARTBEAT,  // A heartbeat packet to keep the connection alive
    CONNECT,    // A packet sent upon connecting
    DISCONNECT, // A packet sent upon disconnection

    GAME_SCREEN,
    PLAYER_LIST,

    // Error packet threating
    PARSING_ERROR // Reserved for any custom packet types
};

// Utility function to convert PacketType to a string (for debugging)
inline const char *PacketTypeToString(PacketType type)
{
    switch (type)
    {
    case PacketType::JOIN_REQUEST:
        return "JOIN_REQUEST";
    case PacketType::JOIN_ACCEPTED:
        return "JOIN_ACCEPTED";
    case PacketType::JOIN_DENIED:
        return "JOIN_DENIED";
    case PacketType::REQUEST_START:
        return "START";
    case PacketType::RESTART:
        return "RESTART";
    case PacketType::PAUSE:
        return "PAUSE";
    case PacketType::RESUME:
        return "RESUME";
    case PacketType::LEFT:
        return "LEFT";
    case PacketType::RIGHT:
        return "RIGHT";
    case PacketType::ROTATE_LEFT:
        return "ROTATE_LEFT";
    case PacketType::ROTATE_RIGHT:
        return "ROTATE_RIGHT";
    case PacketType::DROP_FASTER:
        return "DROP_FASTER";
    case PacketType::DROP_INSTANT:
        return "DROP_INSTANT";
    case PacketType::HEARTBEAT:
        return "HEARTBEAT";
    case PacketType::CONNECT:
        return "CONNECT";
    case PacketType::DISCONNECT:
        return "DISCONNECT";
    case PacketType::PARSING_ERROR:
        return "PARSING_ERROR";
    default:
        return "UNKNOWN_PACKET_TYPE";
    }
}