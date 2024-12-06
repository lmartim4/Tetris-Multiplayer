#pragma once
#include <json.hpp>
#include <string>

class ISerializable
{
public:
    virtual ~ISerializable() = default;
    virtual nlohmann::json serialize() const = 0;
    virtual void deserialize(const nlohmann::json &data) = 0;
};