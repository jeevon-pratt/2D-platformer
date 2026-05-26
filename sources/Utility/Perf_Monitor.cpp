#include <SDL2/SDL_timer.h>             // SDL_GetTicks function
#include <SDL2/SDL_ttf.h>               // _TTF_Font struct

#include <string>                       // std::to_string function

#include "Utility/Math.hpp"             // RoundToInt function
#include "Utility/Perf_Monitor.hpp"     // PerfMonitor class


// **************
// IMPLEMENTATION
// **************

PerfMonitor::PerfMonitor():
    m_currentTime  ( SDL_GetTicks() ),
    m_elapsedTime  (0),
    m_intervalTime (1000)
{
}



void PerfMonitor::SetInterval(uint32_t intervalTime)
{
    m_intervalTime = intervalTime;
}



void PerfMonitor::SetTextFont(_TTF_Font* font)
{
    m_frameRateText.font = font;
    m_frameTimeText.font = font;
}



void PerfMonitor::SetTextColor(SDL_Color color)
{
    m_frameRateText.color = color;
    m_frameTimeText.color = color;
}



void PerfMonitor::CalculateFrameRate()
{
    uint32_t newTime = SDL_GetTicks();
    uint32_t frameTime = (newTime - m_currentTime);

    m_currentTime = SDL_GetTicks();
    m_elapsedTime += frameTime;

    if (m_elapsedTime >= m_intervalTime)
    {
        // frames per second = (1 frame) / (frame time in seconds)
        uint32_t frameRate = RoundToInt<uint32_t>(1000.0f / frameTime);

        m_frameRateText.str = "Frame Rate: " + std::to_string(frameRate) + " fps";
        m_frameTimeText.str = "Frame Time: " + std::to_string(frameTime) + " ms";

        m_elapsedTime = 0;
    }
}



const Text& PerfMonitor::GetFrameRateText() const
{
    return m_frameRateText;
}



const Text& PerfMonitor::GetFrameTimeText() const
{
    return m_frameTimeText;
}
