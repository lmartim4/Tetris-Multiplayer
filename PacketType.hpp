#ifndef PACKET_TYPE_HPP
#define PACKET_TYPE_HPP

enum class PacketType : char
{
    // Game actions/commands
    JOIN_REQUEST = 0,
    JOIN_ACCEPTED,
    JOIN_DENIED,
    LEAVE_WARNING,
    START,
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
    HEARTBEAT = 14,  // A heartbeat packet to keep the connection alive
    CONNECT = 15,    // A packet sent upon connecting
    DISCONNECT = 16, // A packet sent upon disconnection
    CUSTOM = 17      // Reserved for any custom packet types
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
    case PacketType::LEAVE_WARNING:
        return "LEAVE_WARNING";
    case PacketType::START:
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
    case PacketType::CUSTOM:
        return "CUSTOM";
    default:
        return "UNKNOWN_PACKET_TYPE";
    }
}

#endif // PACKET_TYPE_HPP
