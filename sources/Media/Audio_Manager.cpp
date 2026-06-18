#include <SDL3/SDL_timer.h>     // SDL_GetTicks function

#include "Media/Audio.hpp"      // AudioManager class
#include "Utility/Log.hpp"      // GAME_2D_LOG_ERROR macro function
#include "Utility/Math.hpp"     // RoundToInt function
#include "Utility/Log.hpp"      // GAME_2D_LOG_DEBUG and GAME_2D_LOG_ERROR macro functions


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr uint8_t MAX_AUDIO_FILES = 20;



// **************
// IMPLEMENTATION
// **************

AudioManager::AudioManager():
    m_deviceID ( SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr) )
{
    if (!m_deviceID)
        GAME_2D_LOG_ERROR("%s\n\n", SDL_GetError());

    m_data.reserve(MAX_AUDIO_FILES);
}



void AudioManager::LoadAudio(const std::string& name, const std::string& filepath)
{
    GAME_2D_LOG_DEBUG("Loading audio: %s\n\n", filepath.data());


    AudioData data;

    if ( !SDL_LoadWAV(filepath.data(), &data.wavSpec, &data.wavBuffer, &data.wavLength) )
    {
        GAME_2D_LOG_ERROR("Could not load audio, %s\n\n", SDL_GetError());
        return;
    }


    data.stream = SDL_CreateAudioStream(&data.wavSpec, &data.wavSpec);

    if (!data.stream)
    {
        GAME_2D_LOG_ERROR("%s\n\n", SDL_GetError());

        SDL_free(data.wavBuffer);

        data.wavBuffer = nullptr;
        return;
    }

    if ( !SDL_BindAudioStream(m_deviceID, data.stream) )
    {
        GAME_2D_LOG_ERROR("%s\n\n", SDL_GetError());

        SDL_DestroyAudioStream(data.stream);
        SDL_free(data.wavBuffer);

        data.stream    = nullptr;
        data.wavBuffer = nullptr;
        return;
    }


    uint32_t fileSize   = data.wavLength;                               // (in bytes)
    uint32_t channels   = data.wavSpec.channels;                        // (number of channels)
    uint32_t sampleRate = data.wavSpec.freq;                            // (in samples/sec)
    uint32_t sampleSize = SDL_AUDIO_BITSIZE(data.wavSpec.format) / 8;   // (in bytes/sample)

    data.duration  = RoundToInt<uint32_t>( 1000.0f * fileSize / (channels * sampleRate * sampleSize) );
    data.isPlaying = false;

    m_data.emplace(name, data);
}



void AudioManager::PlayAudio(const std::string& name, bool loop)
{
    if ( !m_data.contains(name) )
    {
        GAME_2D_LOG_ERROR("Could not find audio %s\n\n", name.data());
        return;
    }


    AudioData& data = m_data[name];

    if (!data.stream || !data.wavBuffer)
        return;


    if (!data.isPlaying)
    {
        SDL_ClearAudioStream(data.stream);
        SDL_PutAudioStreamData(data.stream, data.wavBuffer, data.wavLength);

        // To initiate the playing of the audio file
        SDL_ResumeAudioDevice(m_deviceID);

        data.startTime = SDL_GetTicks();
        data.isPlaying = true;
    }


    uint16_t elapsedTime = (SDL_GetTicks() - data.startTime);

    if (elapsedTime > data.duration)
    {
        if (!loop)
        {
            SDL_PauseAudioDevice(m_deviceID);

            data.isPlaying = false;
            return;
        }

        // Replay file from the beginning
        SDL_ClearAudioStream(data.stream);
        SDL_PutAudioStreamData(data.stream, data.wavBuffer, data.wavLength);
        SDL_ResumeAudioDevice(m_deviceID);

        data.startTime = SDL_GetTicks();
    }
}



void AudioManager::PauseAudio(const std::string& name)
{
    if ( !m_data.contains(name) )
    {
        GAME_2D_LOG_ERROR("Could not find %s\n\n", name.data());
        return;
    }


    AudioData& data = m_data[name];

    if (data.isPlaying)
    {
        // Stops the playing of the audio file
        SDL_PauseAudioDevice(m_deviceID);

        data.isPlaying = false;
    }
}



void AudioManager::ResetAudio(const std::string& name)
{
    if ( !m_data.contains(name) )
    {
        GAME_2D_LOG_ERROR("Could not find %s\n\n", name.data());
        return;
    }


    AudioData& data = m_data[name];

    if (!data.stream || !data.wavBuffer)
        return;

    SDL_ClearAudioStream(data.stream);
    SDL_PutAudioStreamData(data.stream, data.wavBuffer, data.wavLength );

    data.startTime = SDL_GetTicks();
}



AudioManager::~AudioManager()
{
    for (auto& [name, data] : m_data)
    {
        GAME_2D_LOG_DEBUG("Destroying audio: %s\n\n", name.data());

        SDL_DestroyAudioStream(data.stream);
        SDL_free(data.wavBuffer);
    }

    SDL_CloseAudioDevice(m_deviceID);
}
