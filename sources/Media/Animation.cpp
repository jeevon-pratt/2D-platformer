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
    m_currentTime  (SDL_GetTicks()),
    m_elapsedTime  (0),
    m_currentIndex (0)
{
    m_frames.reserve(MAX_ANIMATION_FRAMES);
}



void Animation::AddFrame(const Frame& frame)
{
    m_frames.emplace_back(frame);
}



bool Animation::FrameTimeExceeded() const
{
    GAME_2D_ASSERT(!m_frames.empty());

    return (m_elapsedTime >= GetCurrentFrame().duration);
}



bool Animation::CycleCompleted() const
{
    GAME_2D_ASSERT(!m_frames.empty());

    uint8_t lastIndex = (m_frames.size() - 1);

    return (m_currentIndex == lastIndex);
}



const Frame& Animation::GetCurrentFrame() const
{
    GAME_2D_ASSERT(!m_frames.empty());

    return m_frames[ m_currentIndex ];
}



void Animation::Reset()
{
    m_currentTime  = SDL_GetTicks();
    m_elapsedTime  = 0;
    m_currentIndex = 0;
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
        m_currentIndex = 0;

    else
        ++m_currentIndex;
}
