#include <SDL3/SDL_events.h>            // SDL_Event union
#include <SDL3/SDL_render.h>            // SDL_Renderer class, SDL_GetRenderWindow function
#include <SDL3/SDL_timer.h>             // SDL_GetTicks function

#include "Utility/Log.hpp"              // GAME_2D_LOG_ERROR macro function
#include "Utility/Math.hpp"             // RoundToInt function
#include "Utility/Perf_Monitor.hpp"     // PerfMonitor class


// **************
// IMPLEMENTATION
// **************

PerfMonitor::PerfMonitor():
    m_currentTime  (SDL_GetTicks()),
    m_elapsedTime  (0),
    m_intervalTime (1000),
    m_frameRate    (0),
    m_frameTime    (0)
{
}



void PerfMonitor::SetInterval(uint32_t intervalTime)
{
    m_intervalTime = intervalTime;
}



void PerfMonitor::CalculateFrameRate()
{
    uint32_t newTime = SDL_GetTicks();
    
    m_frameTime   = (newTime - m_currentTime);
    m_currentTime = SDL_GetTicks();
    m_elapsedTime += m_frameTime;


    if (m_elapsedTime < m_intervalTime)
        return;

    // frames per second = (1 frame) / (frame time in seconds)
    m_frameRate   = RoundToInt<uint32_t>(1000.0f / m_frameTime);
    m_elapsedTime = 0;
}



uint32_t PerfMonitor::GetFrameRate() const
{
    return m_frameRate;
}



uint32_t PerfMonitor::GetFrameTime() const
{
    return m_frameTime;
}
