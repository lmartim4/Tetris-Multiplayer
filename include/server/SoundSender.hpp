#pragma once
#include "SoundType.hpp"
#include "Player.hpp"

class SoundSender
{
public:
    virtual void playSoundToPlayer(const Player *player, SoundType sound) = 0;
    virtual void broadcastSound(SoundType sound) = 0;
    virtual void broadcastBackgroundSound(SoundType sound) = 0;
};