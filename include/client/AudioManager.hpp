#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include "SoundType.hpp"
#include "Logger.hpp"

class AudioManager
{
public:
    AudioManager(){}
    ~AudioManager() = default;

    void playSound(SoundType type);
    void loadAllSounds();

private:
    Logger *logger = new Logger("Audio Manager");
    bool tryLoadSound(SoundType type, const std::string &filepath);
    std::map<SoundType, sf::SoundBuffer> m_soundBuffers;
    std::map<SoundType, sf::Sound> m_sounds;
};
