#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include "ISerializable.hpp"
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
    ROTATE_CCW,
    ROTATE_CW,
    DROP_FASTER,
    HARD_DROP,

    // FROM SERVER TO CLIENT
    GAME_SCREEN,
    PLAYER_LIST,
    GAME_SCORE,
    ENG_GAME_SCREEN,
    PLAY_SOUND,
    NEXT_TETROMINO_DATA,

    // Network/connection-related
    HEARTBEAT,  // A heartbeat packet to keep the connection alive
    CONNECT,    // A packet sent upon connecting
    DISCONNECT, // A packet sent upon disconnection
    SERVER_CLOSED,

    // Error packet threating
    PARSING_ERROR // Reserved for any custom packet types
};

inline std::string PacketTypeToString(PacketType type)
{
    switch (type)
    {
    case PacketType::GAME_SCORE:
        return "GAME_SCORE";
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
    case PacketType::ROTATE_CCW:
        return "ROTATE_CCW";
    case PacketType::ROTATE_CW:
        return "ROTATE_CW";
    case PacketType::DROP_FASTER:
        return "DROP_FASTER";
    case PacketType::HARD_DROP:
        return "DROP_INSTANT";
    case PacketType::HEARTBEAT:
        return "HEARTBEAT";
    case PacketType::CONNECT:
        return "CONNECT";
    case PacketType::DISCONNECT:
        return "DISCONNECT";
    case PacketType::PLAYER_LIST:
        return "PLAYER_LIST";
    case PacketType::PLAY_SOUND:
        return "PLAY_SOUND";
    case PacketType::PARSING_ERROR:
        return "PARSING_ERROR";
    default:
        return "UNKNOWN_PACKET_TYPE (" + std::to_string(static_cast<int>(type)) + ")";
    }
}

class Packet
{
    PacketType type;           // The type of the packet (ACTION, HEARTBEAT, etc.)
    std::vector<uint8_t> data; // Data payload of the packet
    ENetPeer *peer;            // The peer that the packet is being sent to or received from

public:
    Packet(PacketType t, ENetPeer *dest = nullptr)
        : type(t), peer(dest) {}

    Packet(PacketType t, const std::vector<uint8_t> &d, ENetPeer *dest = nullptr) : type(t), data(d), peer(dest) {}
    Packet(PacketType t, int i, ENetPeer *dest = nullptr) : type(t), peer(dest) { data.emplace_back(i); }

    Packet(PacketType t, const ISerializable &serializable, ENetPeer *dest = nullptr)
        : type(t), peer(dest)
    {
        std::string jsonStr = serializable.serialize().dump();
        data.assign(jsonStr.begin(), jsonStr.end());
    }

    // Serialize the packet to raw byte data so it can be sent over the network.
    std::vector<uint8_t> toRawPacket() const
    {
        std::vector<uint8_t> rawData;
        rawData.push_back(static_cast<uint8_t>(type));           // First byte stores the packet type
        rawData.insert(rawData.end(), data.begin(), data.end()); // Append the rest of the data
        return rawData;
    }

    void setENetPeer(ENetPeer *p)
    {
        peer = p;
    }

    const std::vector<uint8_t> &getData() const
    {
        return data;
    }

    nlohmann::json getPayloadAsJson() const
    {
        std::string jsonStr(data.begin(), data.end());
        return nlohmann::json::parse(jsonStr);
    }

    PacketType getType() const
    {
        return type;
    }

    ENetPeer *getPeer() const
    {
        return peer;
    }
};