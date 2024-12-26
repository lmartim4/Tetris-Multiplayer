#include "AudioManager.hpp"
#include <iostream>

bool AudioManager::loadSound(SoundType type, const std::string &filepath)
{
    console_log("Loading sound: " + filepath);

    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(filepath))
    {
        console_log("Failed to load file: " + filepath);
        return false;
    }

    m_soundBuffers[type] = buffer;
    m_sounds[type].setBuffer(m_soundBuffers[type]);

    return true;
}

void AudioManager::playSound(SoundType type)
{
    auto it = m_sounds.find(type);
    if (it == m_sounds.end())
    {
        console_log("SoundType not loaded: " + static_cast<int>(type));
        return;
    }
    it->second.play();
}

void AudioManager::loadAllSounds()
{
    for (int i = 0; i < static_cast<int>(SoundType::Count); i++)
    {
        SoundType c = static_cast<SoundType>(i);
        std::string path = toFilePath(c);

        if (!loadSound(c, path))
            console_log("Could not load: " + path);
    }
}
