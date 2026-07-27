#include <json/value.h>             // Json::Value class
#include <SDL3/SDL_audio.h>         // SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK macro
#include <SDL3_mixer/SDL_mixer.h>   // SDL_Mixer functionality

#include "Media/Audio.hpp"          // Audio classes
#include "Media/Asset.hpp"          // AudioManager class
#include "Utility/Assert.hpp"       // GAME_2D_ASSERT macro function
#include "Utility/Log.hpp"          // Log macro functions


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr uint8_t MAX_AUDIO_DATA = 20;



// **************
// IMPLEMENTATION
// **************

AudioManager::AudioManager():
    m_mixer (MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr))
{
    if (!m_mixer)
    {
        GAME_2D_LOG_ERROR("%s\n", SDL_GetError());
        return;
    }

    m_registry.reserve(MAX_AUDIO_DATA);
}



void AudioManager::LoadAudio(const Json::Value& asset)
{
    const char* name = asset["name"].asCString();
    const char* path = asset["path"].asCString();
    const bool  flag = asset["predecode"].asBool();

    GAME_2D_LOG_DEBUG("Loading audio: %s\n", path);

    MIX_Audio* data = MIX_LoadAudio(m_mixer, path, flag);
    
    if (!data)
    {
        GAME_2D_LOG_ERROR("%s\n", SDL_GetError());
        return;
    }

    m_registry.emplace(name, data);

    GAME_2D_ASSERT(m_registry.size() <= MAX_AUDIO_DATA);
}



Audio AudioManager::GetAudio(const std::string& name) const
{
    if (!m_registry.contains(name))
    {
        GAME_2D_LOG_ERROR("Could not find audio %s\n", name.c_str());
        return Audio();
    }

    MIX_Audio* data  = m_registry.at(name);
    MIX_Track* track = MIX_CreateTrack(m_mixer);

    MIX_SetTrackAudio(track, data);

    return Audio(track);
}



AudioManager::~AudioManager()
{
    for (auto& [name, data] : m_registry)
    {
        GAME_2D_LOG_DEBUG("Destroying audio: %s\n", name.c_str());
        MIX_DestroyAudio(data);
    }

    MIX_DestroyMixer(m_mixer);
}
