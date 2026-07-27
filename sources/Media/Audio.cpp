#include <SDL3_mixer/SDL_mixer.h>   // SDL_Mixer functionality

#include "Media/Audio.hpp"          // Audio class
#include "Utility/Log.hpp"          // GAME_2D_LOG_ERROR macro function


// **************
// IMPLEMENTATION
// **************

Audio::Audio():
    m_track (nullptr)
{
}



Audio::Audio(MIX_Track* track):
    m_track (track)
{
}



Audio::Audio(Audio&& audio) noexcept:
    m_track (audio.m_track)
{
    audio.m_track = nullptr;
}



void Audio::operator=(Audio&& audio) noexcept
{
    if (this == &audio)
        return;

    if (m_track)
        MIX_DestroyTrack(m_track);

    m_track = audio.m_track;

    audio.m_track = nullptr;
}



void Audio::Play()
{
    if (!m_track)
        return;

    if (MIX_TrackPlaying(m_track))
        return;

    MIX_SetTrackLoops(m_track, 0);
    MIX_PlayTrack(m_track, 0);
}



void Audio::PlayLoop()
{
    if (!m_track)
        return;

    if (MIX_TrackPlaying(m_track))
        return;

    MIX_SetTrackLoops(m_track, -1);
    MIX_PlayTrack(m_track, 0);
}



void Audio::Pause()
{
    if (m_track)
        MIX_PauseTrack(m_track);
}



void Audio::Reset()
{
    if (m_track)
        MIX_StopTrack(m_track, 0);
}



void Audio::SetVolume(float gain)
{
    if (m_track)
        MIX_SetTrackGain(m_track, gain);
}



Audio::~Audio()
{
    if (m_track)
        MIX_DestroyTrack(m_track);
}
