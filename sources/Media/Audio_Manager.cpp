#include <SDL2/SDL_timer.h>     // SDL_GetTicks function

#include <iostream>             // std::cout

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

AudioManager::AudioManager()
{
    m_data.reserve(MAX_AUDIO_FILES);
}



void AudioManager::LoadAudio(std::string_view name, std::string_view filepath)
{
    GAME_2D_LOG_DEBUG("Loading audio: %s\n\n", filepath.data());


    AudioData& data = m_data[ name.data() ];

    if ( !SDL_LoadWAV(filepath.data(), &data.wavSpec, &data.wavBuffer, &data.wavLength) )
    {
        GAME_2D_LOG_ERROR("%s\n\n", SDL_GetError());
        return;
    }


    uint32_t fileSize   = data.wavLength;                               // (in bytes)
    uint32_t channels   = data.wavSpec.channels;                        // (number of channels)
    uint32_t sampleRate = data.wavSpec.freq;                            // (in samples/sec)
    uint32_t sampleSize = SDL_AUDIO_BITSIZE(data.wavSpec.format) / 8;   // (in bytes/sample)

    data.deviceID  = SDL_OpenAudioDevice(nullptr, 0, &data.wavSpec, nullptr, 0);
    data.duration  = RoundToInt<uint32_t>( 1000.0f * fileSize / (channels * sampleRate * sampleSize) );
    data.isPlaying = false;

    SDL_QueueAudio(data.deviceID, data.wavBuffer, data.wavLength);
}



void AudioManager::PlayAudio(std::string_view name, bool loopEnabled)
{
    if ( m_data.count(name.data()) == 0 )
    {
        GAME_2D_LOG_ERROR("Could not find audio %s\n\n", name.data());
        return;
    }


    AudioData& data = m_data[ name.data() ];

    if (!data.isPlaying)
    {
        // To initiate the playing of the audio file
        SDL_PauseAudioDevice(data.deviceID, 0);

        data.startTime = SDL_GetTicks();
        data.isPlaying = true;
    }


    uint16_t elapsedTime = (SDL_GetTicks() - data.startTime);

    if (elapsedTime > data.duration)
    {
        if (!loopEnabled)
        {
            data.isPlaying = false;

            return;
        }

        // Replay file from the beginning
        ResetAudio(name);
        SDL_PauseAudioDevice(data.deviceID, 0);

        data.startTime = SDL_GetTicks();
    }
}



void AudioManager::PauseAudio(std::string_view name)
{
    if ( m_data.count(name.data()) == 0 )
    {
        GAME_2D_LOG_ERROR("Could not find %s\n\n", name.data());
        return;
    }


    AudioData& data = m_data[ name.data() ];

    // Stops the playing of the audio file
    if (data.isPlaying)
    {
        SDL_PauseAudioDevice(data.deviceID, 1);

        data.isPlaying = false;
    }
}



void AudioManager::ResetAudio(std::string_view name)
{
    if ( m_data.count(name.data()) == 0 )
    {
        GAME_2D_LOG_ERROR("Could not find %s\n\n", name.data());
        return;
    }


    AudioData& data = m_data[ name.data() ];

    SDL_ClearQueuedAudio(data.deviceID);
    SDL_QueueAudio( data.deviceID, data.wavBuffer, data.wavLength );
}



AudioManager::~AudioManager()
{
    for (auto& [name, data] : m_data)
    {
        GAME_2D_LOG_DEBUG("Destroying audio: %s\n\n", name.data());

        SDL_CloseAudioDevice(data.deviceID);
        SDL_FreeWAV(data.wavBuffer);
    }
}
