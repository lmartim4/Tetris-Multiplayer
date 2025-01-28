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

void AudioManager::playSound(SoundType type, bool loop)
{
    if (type == SoundType::STOP)
    {
        stopBackgroundMusic(); // Stops background music if `STOP` is passed
        return;
    }

    auto it = m_sounds.find(type);
    if (it == m_sounds.end())
    {
        logger->console_log("SoundType not loaded: " + std::to_string(static_cast<int>(type)));
        return;
    }

    // If this is a new background sound, stop any currently playing one
    if (loop)
    {
        stopBackgroundMusic();
        m_currentBackgroundSound = type; // Update the current background sound
    }

    // Set looping and play the sound
    it->second.setLoop(loop);
    it->second.play();
    // logger->console_log("(+) Playing sound: " + std::to_string(static_cast<int>(type)) + " with loop set to: " + std::string(loop ? "true" : "false"));
}

void AudioManager::stopBackgroundMusic()
{
    auto it = m_sounds.find(m_currentBackgroundSound);
    if (it == m_sounds.end())
    {
        // logger->console_log("No background music to stop.");
        return;
    }

    it->second.stop(); // Stop the currently playing background sound
    // logger->console_log("(-) Background music stopped: " + std::to_string(static_cast<int>(m_currentBackgroundSound)));
    m_currentBackgroundSound = SoundType::STOP; // Reset the background sound type
}

void AudioManager::loadAllSounds()
{
    for (int i = 0; i < static_cast<int>(SoundType::STOP); i++)
    {
        SoundType c = static_cast<SoundType>(i);
        std::string path = toFilePath(c);
        tryLoadSound(c, path);
    }
}
