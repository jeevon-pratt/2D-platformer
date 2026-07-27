#pragma once

struct MIX_Track;


/**
 *  Structure for implementing sound effects
 */
class Audio
{
public:
    // IMPLEMENTATION
    // ==============

    // Default Constructor
    Audio();

    // Audio Track Constructor
    Audio(MIX_Track* track);

    // Move Constructor
    Audio(Audio&& audio) noexcept;

    // Move Assignment Operator
    void operator=(Audio&& audio) noexcept;

    // Plays the audio track once
    void Play();

    // Plays the audio track on a loop
    void PlayLoop();

    // Pauses the audio
    void Pause();

    // Resets the audio from the beginning
    void Reset();

    // Sets the volume of the audio
    void SetVolume(float gain);

    // Destructor
    ~Audio();

private:
    // Pointer to the SDL_Mixer audio track
    MIX_Track* m_track;
};
