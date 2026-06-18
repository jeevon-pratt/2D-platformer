#pragma once

#include <SDL3/SDL_audio.h>     // SDL_AudioStream, SDL_AudioSpec, SDL_AudioDeviceID

#include <string>               // std::string
#include <unordered_map>        // std::unordered_map


/**
 *  Structure that represents .wav file meta data
 */
struct AudioData
{
    SDL_AudioStream* stream;       // A pointer to the audio stream
    SDL_AudioSpec    wavSpec;      // Audio device properties
    uint8_t*         wavBuffer;    // A pointer to the audio data buffer
    uint32_t         wavLength;    // Length of audio data buffer in bytes
    uint32_t         startTime;    // Variable that manages playing duration of audio
    uint32_t         duration;     // Audio file duration in milliseconds
    bool             isPlaying;    // Indicates if audio file is currently playing
};



/**
 *  Class for implementing game audio
 */
class AudioManager final
{
public:
    // IMPLEMENTATION
    // ==============

    // Default constructor
    AudioManager();

    // Loads audio data with the specified file path
    void LoadAudio(const std::string& name, const std::string& filepath);

    // Plays the audio file with the specified name
    //
    // Note: 1) This method will continuously play the audio file whether called
    //          inside or outside the game loop.
    //
    //       2) If looping  is not enabled the 'ResetAudio' method would have to be
    //          called in order to replay the audio file from the beginning.
    void PlayAudio(const std::string& name, bool loopEnabled = true);

    // Pauses the audio file with the specified name
    void PauseAudio(const std::string& name);

    // Used for replaying the audio file with the specified name
    void ResetAudio(const std::string& name);

    // Closes audio devices and frees all audio data loaded by the manager
    ~AudioManager();

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and assignment operator have been disabled to
    //       prevent the copying of audio assets.
    AudioManager(const AudioManager& manager) = delete;
    void operator=(const AudioManager& manager) = delete;

private:
    // The playback device ID
    SDL_AudioDeviceID m_deviceID;

    // Hash table that maps all the loaded audio data to a string name
    //
    // Note: The destructor automatically destroys all loaded audio data. This is
    //       handled by the main game class.
    std::unordered_map<std::string, AudioData> m_data;
};
