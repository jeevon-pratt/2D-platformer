#pragma once

#include <SDL2/SDL_audio.h>     // SDL_AudioDeviceID typedef and SDL_AudioSpec struct

#include <string>               // std::string
#include <string_view>          // std::string_view
#include <unordered_map>        // std::unordered_map


/**
 *  Structure that represents .wav file meta data
 */
struct AudioData
{
    SDL_AudioDeviceID deviceID;     // Audio device ID
    SDL_AudioSpec     wavSpec;      // Audio device properties
    uint8_t*          wavBuffer;    // A pointer to the audio data buffer
    uint32_t          wavLength;    // Length of audio data buffer in bytes
    uint32_t          startTime;    // Variable that manages playing duration of audio
    uint32_t          duration;     // Audio file duration in milliseconds
    bool              isPlaying;    // Indicates if audio file is currently playing
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
    void LoadAudio(std::string_view filepath);

    // Plays the audio file with the specified file path
    //
    // Note: 1) This method will continuously play the audio file whether called
    //          inside or outside the game loop.
    //
    //       2) If looping  is not enabled the 'ResetAudio' method would have to be
    //          called in order to replay the audio file from the beginning.
    void PlayAudio(std::string_view filepath, bool loopEnabled = true);

    // Pauses the audio file with the specified filepath
    void PauseAudio(std::string_view fileapth);

    // Used for replaying the audio file with the specified file path
    void ResetAudio(std::string_view filepath);

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
    // Hash table that maps all the loaded audio data to its file path
    //
    // Note: The destructor automatically destroys all loaded audio data. This is
    //       handled by the main game class.
    std::unordered_map<std::string, AudioData> m_data;
};
