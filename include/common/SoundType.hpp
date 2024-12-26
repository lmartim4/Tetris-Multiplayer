#pragma once

#include <string>

enum class SoundType : int
{
    Seeds,
    DjembeSlap,
    DjembeBass,
    Lv256,
    Lv4,
    Lv3,
    Lv2,
    Lv1,
    DeathSound,
    WinSound,
    Count
};

inline std::string toFilePath(SoundType type)
{
    switch (type)
    {
    case SoundType::Seeds:
        return "assets/sounds/542752__panpiper5__seeds.wav";

    case SoundType::DjembeSlap:
        return "assets/sounds/659910__panpiper5__djembe-slap.wav";

    case SoundType::DjembeBass:
        return "assets/sounds/659911__panpiper5__djembe-bass.wav";

    case SoundType::Lv256:
        return "assets/sounds/509683__nutetoonstudios__lv-256.wav";

    case SoundType::Lv4:
        return "assets/sounds/507981__nutetoonstudios__lv-4.wav";

    case SoundType::Lv3:
        return "assets/sounds/507980__nutetoonstudios__lv-3.wav";

    case SoundType::Lv2:
        return "assets/sounds/507979__nutetoonstudios__lv-2.wav";

    case SoundType::Lv1:
        return "assets/sounds/507978__nutetoonstudios__lv-1.wav";

    case SoundType::DeathSound:
        return "assets/sounds/507977__nutetoonstudios__death-sound.wav";

    case SoundType::WinSound:
        return "assets/sounds/507976__nutetoonstudios__win-sound.wav";
    }

    // Fallback (in case of an undefined enum)
    return {};
}
