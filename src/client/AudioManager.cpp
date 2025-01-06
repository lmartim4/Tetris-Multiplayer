#include "AudioManager.hpp"
#include <iostream>

bool AudioManager::tryLoadSound(SoundType type, const std::string &filepath)
{
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(filepath))
    {
        logger->console_log("(-) Failed to load file: " + filepath);
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
        logger->console_log("SoundType not loaded: " + std::to_string(static_cast<int>(type)));
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
        tryLoadSound(c, path);
    }
}
