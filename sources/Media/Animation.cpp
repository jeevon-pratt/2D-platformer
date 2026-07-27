#include <SDL3/SDL_timer.h>       // SDL_GetTicks function

#include "Media/Animation.hpp"    // Animation class and Frame class
#include "Utility/Assert.hpp"     // GAME_2D_ASSERT macro function


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr uint8_t MAX_ANIMATION_FRAMES = 100;



// **************
// IMPLEMENTATION
// **************

Animation::Animation():
    m_currentTime (SDL_GetTicks()),
    m_elapsedTime (0),
    m_index       (0)
{
    m_frames.reserve(MAX_ANIMATION_FRAMES);
}



void Animation::AddFrame(Frame frame)
{
    m_frames.emplace_back(frame);

    GAME_2D_ASSERT( m_frames.size() < MAX_ANIMATION_FRAMES );
}



bool Animation::FrameTimeExceeded() const
{
    if (m_frames.empty())
        return false;

    return (m_elapsedTime >= GetCurrentFrame().duration);
}



bool Animation::CycleCompleted() const
{
    if (m_frames.empty())
        return false;

    uint8_t lastIndex = (m_frames.size() - 1);

    return (m_index == lastIndex);
}



Frame Animation::GetCurrentFrame() const
{
    if (m_frames.empty())
        return Frame();

    return m_frames[ m_index ];
}



void Animation::Reset()
{
    m_currentTime = SDL_GetTicks();
    m_elapsedTime = 0;
    m_index       = 0;
}



void Animation::Play()
{
    // To display current frame until duration has been exceeded
    uint32_t newTime  = SDL_GetTicks();
    uint32_t timeStep = (newTime - m_currentTime);

    m_currentTime = newTime;
    m_elapsedTime += timeStep;

    if (!FrameTimeExceeded())
        return;


    // To progress to the next frame of the animation cycle
    m_elapsedTime = 0;

    if (CycleCompleted())
        m_index = 0;

    else
        ++m_index;
}
