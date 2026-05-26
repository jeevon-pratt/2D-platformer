#include <SDL2/SDL_timer.h>     // SDL_GetTicks function

#include "Media/Animation.hpp"  // Animation class and Frame class


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr uint8_t MAX_ANIMATION_FRAMES = 100;



// **************
// IMPLEMENTATION
// **************

Animation::Animation():
    m_cycleComplete (false),
    m_currentTime   (0),
    m_elapsedTime   (0),
    m_currentIndex  (0)
{
    m_frames.reserve(MAX_ANIMATION_FRAMES);
}



const Frame& Animation::GetCurrentFrame() const
{
    return m_frames[ m_currentIndex ];
}



void Animation::AddFrame(const Frame& frame)
{
    m_frames.push_back(frame);
}



void Animation::NextFrame()
{
    if ( m_currentIndex< (m_frames.size() - 1) )
    {
        m_cycleComplete = false;
        ++m_currentIndex;
    }
    else
    {
        m_cycleComplete = true;
        m_currentIndex = 0;
    }

    m_currentTime = SDL_GetTicks();
    m_elapsedTime = 0;
}



void Animation::StepTime()
{
    uint8_t dt = (SDL_GetTicks() - m_currentTime);

    m_elapsedTime += dt;
}



bool Animation::FrameTimeExceeded() const
{
    return (m_elapsedTime >= GetCurrentFrame().duration);
}



bool Animation::CycleComplete() const
{
    return m_cycleComplete;
}
