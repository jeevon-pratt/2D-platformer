#include <SDL2/SDL_events.h>            // SDL_Event union
#include <SDL2/SDL_render.h>            // SDL_Renderer class, SDL_RenderGetWindow function
#include <SDL2/SDL_timer.h>             // SDL_GetTicks function

#include "imgui.h"                      // ImGui functionality
#include "imgui_impl_sdl2.h"            // ImGui_ImplSDL2 functionality
#include "imgui_impl_sdlrenderer2.h"    // ImGui_ImplSDLRenderer2 functionality

#include "Media/Renderer.hpp"           // Renderer class
#include "Utility/Log.hpp"              // GAME_2D_LOG_ERROR macro function
#include "Utility/Math.hpp"             // RoundToInt function
#include "Utility/Perf_Monitor.hpp"     // PerfMonitor class


// **************
// IMPLEMENTATION
// **************

PerfMonitor::PerfMonitor(const Renderer& renderer):
    m_rendererContext (renderer.GetContext()),
    m_guiContext      (nullptr),
    m_backend         (false),
    m_currentTime     (SDL_GetTicks()),
    m_elapsedTime     (0),
    m_intervalTime    (1000),
    m_frameRate       (0),
    m_frameTime       (0)
{
    // Initialization of ImGui
    // =======================

    IMGUI_CHECKVERSION();

    m_guiContext = ImGui::CreateContext();

    if (!m_guiContext)
    {
        GAME_2D_LOG_ERROR("Failed to create the ImGui context\n\n");
        return;
    }

    ImGui::SetCurrentContext(m_guiContext);
    ImGui::GetIO().FontGlobalScale = 2.0f;


    // Initialization of SDL2 Backend
    // ===============================

    SDL_Renderer* rendererContext = const_cast<SDL_Renderer*>( renderer.GetContext() );
    SDL_Window*   windowContext   = SDL_RenderGetWindow(rendererContext);

    if ( !ImGui_ImplSDL2_InitForSDLRenderer(windowContext, rendererContext) )
    {
        GAME_2D_LOG_ERROR("Failed to initialize ImGui SDL2 backend\n\n");
        ImGui::DestroyContext(m_guiContext);

        m_guiContext = nullptr;
        return;
    }


    // Initialization of SDL2 Renderer Backend
    // =======================================

    if ( !ImGui_ImplSDLRenderer2_Init(rendererContext) )
    {
        GAME_2D_LOG_ERROR("Failed to initialize ImGui SDL2 Renderer backend\n\n");
        
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext(m_guiContext);

        m_guiContext = nullptr;
        return;
    }


    // To set this flag true upon successful initialization
    m_backend = true;
}



void PerfMonitor::SetInterval(uint32_t intervalTime)
{
    m_intervalTime = intervalTime;
}



void PerfMonitor::SetCurrentContext()
{
    if (!m_guiContext)
        return;

    ImGui::SetCurrentContext(m_guiContext);
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





void PerfMonitor::HandleInput(const SDL_Event& event) const
{
    if (!m_backend)
        return;

    ImGui_ImplSDL2_ProcessEvent(&event);
}




void PerfMonitor::Render() const
{
    if (!m_backend)
        return;

    SDL_Renderer* rendererContext = const_cast<SDL_Renderer*>(m_rendererContext);

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Performance Monitor");
    ImGui::Text("Frame Rate: %u FPS", m_frameRate);
    ImGui::Text("Frame Time: %u ms", m_frameTime);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), rendererContext);
}



PerfMonitor::~PerfMonitor()
{
    if (!m_guiContext)
        return;

    ImGui::SetCurrentContext(m_guiContext);

    if (m_backend)
    {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
    }

    ImGui::DestroyContext(m_guiContext);
}
