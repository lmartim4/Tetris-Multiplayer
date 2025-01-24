#pragma once

#include <string>

constexpr const char audio_path[] = "../assets/sounds/";

enum class SoundType : int
{
    LevelUp,
    BreakLine,
    Seeds,
    DjembeSlap,
    DjembeBass,
    Lv256,
    Lv4,
    Lv3,
    Lv2,
    Lv1,
    OnJoinLobby,
    DeathSound,
    WinSound,
    FallSound,
    DenyErrorSound,
    FabricImpactSound,

    /* Always leave it as the last one */
    Count
};

inline std::string toFilePath(SoundType type)
{
    switch (type)
    {
    case SoundType::OnJoinLobby:
        return std::string(audio_path) + "425165__varnsworth__videogamerisingsfxpart2.mp3";

    case SoundType::LevelUp:
        return std::string(audio_path) + "682633__bastianhallo__level-up.ogg";

    case SoundType::BreakLine:
        return std::string(audio_path) + "41348__datasoundsample__glass-shatter.wav";

    case SoundType::Seeds:
        return std::string(audio_path) + "542752__panpiper5__seeds.wav";

    case SoundType::DjembeSlap:
        return std::string(audio_path) + "659910__panpiper5__djembe-slap.wav";

    case SoundType::DjembeBass:
        return std::string(audio_path) + "659911__panpiper5__djembe-bass.wav";

    case SoundType::Lv256:
        return std::string(audio_path) + "509683__nutetoonstudios__lv-256.wav";

    case SoundType::Lv4:
        return std::string(audio_path) + "507981__nutetoonstudios__lv-4.wav";

    case SoundType::Lv3:
        return std::string(audio_path) + "507980__nutetoonstudios__lv-3.wav";

    case SoundType::Lv2:
        return std::string(audio_path) + "507979__nutetoonstudios__lv-2.wav";

    case SoundType::Lv1:
        return std::string(audio_path) + "507978__nutetoonstudios__lv-1.wav";

    case SoundType::DeathSound:
        return std::string(audio_path) + "507977__nutetoonstudios__death-sound.wav";

    case SoundType::WinSound:
        return std::string(audio_path) + "507976__nutetoonstudios__win-sound.wav";

    case SoundType::FallSound:
        return std::string(audio_path) + "538151__fupicat__8bit-fall.wav";

    case SoundType::DenyErrorSound:
        return std::string(audio_path) + "551543__philracoindie__8-bit-denyerror-sound.wav";

    case SoundType::FabricImpactSound:
        return std::string(audio_path) + "777621__aquaash__fabricimpact.wav";
    }

    // Fallback (in case of an undefined enum)
    return {};
}
