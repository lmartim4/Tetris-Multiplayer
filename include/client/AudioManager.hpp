#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include "SoundType.hpp"
#include "Debuggable.hpp"

class AudioManager
{
public:
    AudioManager(){}
    ~AudioManager() = default;

    void playSound(SoundType type);
    void loadAllSounds();

private:
    Debuggable *logger = new Debuggable("Audio Manager");
    bool tryLoadSound(SoundType type, const std::string &filepath);
    std::map<SoundType, sf::SoundBuffer> m_soundBuffers;
    std::map<SoundType, sf::Sound> m_sounds;
};
