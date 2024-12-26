#pragma once

#include <cstdint>
#include <vector>
#include "enet/enet.h"
#include "json.hpp"

enum class PacketType
{
    // FROM CLIENT TO SERVER
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

    // FROM SERVER TO CLIENT
    GAME_SCREEN,
    PLAYER_LIST,
    ENG_GAME_SCREEN,
    PLAY_SOUND,
    
    
    // Network/connection-related
    HEARTBEAT,  // A heartbeat packet to keep the connection alive
    CONNECT,    // A packet sent upon connecting
    DISCONNECT, // A packet sent upon disconnection

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

struct Packet
{
    PacketType type;           // The type of the packet (ACTION, HEARTBEAT, etc.)
    std::vector<uint8_t> data; // Data payload of the packet
    ENetPeer *peer;            // The peer that the packet is being sent to or received from

    // Constructor to initialize a packet with its type, data, and optional destination peer.
    Packet(PacketType t, const std::vector<uint8_t> &d, ENetPeer *dest = nullptr)
        : type(t), data(d), peer(dest) {}

    // New constructor that accepts a JSON object
    Packet(PacketType t, const nlohmann::json &j, ENetPeer *dest = nullptr)
        : type(t), peer(dest)
    {
        // Convert JSON to string
        std::string jsonStr = j.dump();
        // Store as bytes
        data.assign(jsonStr.begin(), jsonStr.end());
    }

    // Serialize the packet to raw byte data so it can be sent over the network.
    std::vector<uint8_t> toRawData() const
    {
        std::vector<uint8_t> rawData;
        rawData.push_back(static_cast<uint8_t>(type));           // First byte stores the packet type
        rawData.insert(rawData.end(), data.begin(), data.end()); // Append the rest of the data
        return rawData;
    }

    // Helper method to parse the data vector into a JSON object
    nlohmann::json toJson() const
    {
        std::string jsonStr(data.begin(), data.end());
        return nlohmann::json::parse(jsonStr);
    }
};