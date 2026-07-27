#include <SDL3/SDL_render.h>            // SDL Renderer functions

#include <cstdlib>                      // std::exit, EXIT_FAILURE
#include <memory>                       // std::unique_ptr
#include <numbers>                      // std::numbers::pi

#include "imgui.h"                      // ImGui functionality
#include "imgui_impl_sdl3.h"            // ImGui_ImplSDL3 functionality
#include "imgui_impl_sdlrenderer3.h"    // ImGui_ImplSDLRenderer3 functionality

#include "Entity/Game_Object.hpp"       // GameObject class
#include "Media/Renderer.hpp"           // Renderer class
#include "Media/Sprite.hpp"             // Sprite class
#include "Media/Window.hpp"             // Window class
#include "UI/Menu.hpp"                  // Menu class
#include "UI/Widget.hpp"                // Widget classes
#include "Utility/Log.hpp"              // Log macro functions
#include "Utility/Math.hpp"             // RoundToInt and ConvertToScreenCoord functions
#include "Utility/Perf_Monitor.hpp"     // PerfMonitor class


// **************
// IMPLEMENTATION
// **************

Renderer::Renderer(const Window& window):
    m_rendererContext (nullptr)
{
    // Initialization of SDL Renderer
    // ==============================

    GAME_2D_LOG_DEBUG("Initializing SDL3 renderer context\n");

    SDL_Window*   windowContext   = const_cast<SDL_Window*>( window.GetContext() );
    SDL_Renderer* rendererContext = SDL_CreateRenderer(windowContext, nullptr);

    if (!rendererContext)
    {
        GAME_2D_LOG_CRITICAL("%s\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }

    m_rendererContext = rendererContext;



    // Initialization of ImGui
    // =======================

    GAME_2D_LOG_DEBUG("Initializing ImGui context\n");

    IMGUI_CHECKVERSION();

    if (!ImGui::CreateContext())
    {
        GAME_2D_LOG_CRITICAL("Failed to create the ImGui context\n");
        std::exit(EXIT_FAILURE);
    }

    ImGui::GetIO().FontGlobalScale = 2.0f;



    // Initialization of SDL3 Backend
    // ===============================

    GAME_2D_LOG_DEBUG("Initializing ImGui SDL3 backend\n");

    if (!ImGui_ImplSDL3_InitForSDLRenderer(windowContext, rendererContext))
    {
        GAME_2D_LOG_CRITICAL("Failed to initialize ImGui SDL3 backend\n");
        std::exit(EXIT_FAILURE);
    }



    // Initialization of SDL3 Renderer Backend
    // =======================================

    GAME_2D_LOG_DEBUG("Initializing ImGui SDL3 renderer backend\n");

    if (!ImGui_ImplSDLRenderer3_Init(rendererContext))
    {
        GAME_2D_LOG_CRITICAL("Failed to initialize ImGui SDL3 Renderer backend\n");
        std::exit(EXIT_FAILURE);
    }
}



const SDL_Renderer* Renderer::GetContext() const
{
    return m_rendererContext;
}



void Renderer::HandleInput(const SDL_Event& event)
{
    ImGui_ImplSDL3_ProcessEvent(&event);
}



void Renderer::Clear()
{
    SDL_RenderClear(m_rendererContext);
}



void Renderer::DrawGradient(SDL_Color color1, SDL_Color color2, float steps)
{
    int winWidth  = 0;
    int winHeight = 0;

    SDL_GetCurrentRenderOutputSize(m_rendererContext, &winWidth, &winHeight);


    float yStep = winHeight / steps;
    float rStep = (color2.r - color1.r) / steps;
    float gStep = (color2.g - color1.g) / steps;
    float bStep = (color2.b - color1.b) / steps;

    SDL_FRect dstrect;
    dstrect.x = 0.0f;
    dstrect.y = 0.0f;
    dstrect.w = winWidth;
    dstrect.h = yStep;


    for (uint64_t count = 0; count <= steps; ++count)
    {
        SDL_SetRenderDrawColor(m_rendererContext, color1.r, color1.g, color1.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRects(m_rendererContext, &dstrect, 1);

        dstrect.y += yStep;
        color1.r  += RoundToInt<int16_t>(rStep);
        color1.g  += RoundToInt<int16_t>(gStep);
        color1.b  += RoundToInt<int16_t>(bStep);
    }
}



void Renderer::Render(const Sprite& sprite, b2Vec2 position, b2Vec2 camTransform)
{
    b2Vec2 screenCoord = position;

    if (!sprite.UseScreenCoord())
        screenCoord = ConvertToScreenCoord(sprite, position, camTransform);

    SDL_FRect dstrect;
    dstrect.x = screenCoord.x;
    dstrect.y = screenCoord.y;
    dstrect.w = sprite.GetFrameWidth();
    dstrect.h = sprite.GetFrameHeight();

    SDL_RenderTexture(m_rendererContext,
                      const_cast<SDL_Texture*>(sprite.GetTexture()),
                      &sprite.GetSourceRect(),
                      &dstrect);
}



void Renderer::Render(const GameObject& object, b2Vec2 camTransform)
{
    b2Vec2 screenCoord = ConvertToScreenCoord(object.GetSprite(), object.GetPosition(), camTransform);

    SDL_FRect dstrect;
    dstrect.x = screenCoord.x;
    dstrect.y = screenCoord.y;
    dstrect.w = object.GetFrameWidth();
    dstrect.h = object.GetFrameHeight();

    SDL_RenderTextureRotated(m_rendererContext,
                             const_cast<SDL_Texture*>( object.GetTexture() ),
                             &object.GetSourceRect(),
                             &dstrect,
                             -object.GetAngle() * (180.0 / std::numbers::pi),
                             nullptr,
                             object.IsInverted() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}



void Renderer::Render(const Text& text)
{
   m_textRenderer.Render(m_rendererContext, text);
}



void Renderer::Render(const Button& button)
{
    SDL_Texture* icon = const_cast<SDL_Texture*>(button.icon);

    auto [r, g, b, _] = button.GetColorMod();

    SDL_FRect dstrect;
    dstrect.x = button.GetPosition().x;
    dstrect.y = button.GetPosition().y;
    dstrect.w = button.GetWidth();
    dstrect.h = button.GetHeight();

    SDL_SetTextureColorMod(icon, r, g, b);
    SDL_RenderTexture(m_rendererContext, icon, nullptr, &dstrect);
    SDL_SetTextureColorMod(icon, 255, 255, 255);
}



void Renderer::Render(const Slider& slider)
{
    SDL_Texture* trackIcon = const_cast<SDL_Texture*>(slider.trackIcon);
    SDL_Texture* knobIcon  = const_cast<SDL_Texture*>(slider.knobIcon);

    SDL_FRect dstrect;
    dstrect.x = slider.GetPosition().x;
    dstrect.y = slider.GetPosition().y;
    dstrect.w = slider.GetWidth();
    dstrect.h = slider.GetHeight();

    SDL_RenderTexture(m_rendererContext, trackIcon, nullptr, &dstrect);

    dstrect.x = slider.GetKnobPosition().x;
    dstrect.y = slider.GetKnobPosition().y;
    dstrect.w = slider.GetKnobWidth();
    dstrect.h = slider.GetKnobHeight();

    SDL_RenderTexture(m_rendererContext, knobIcon, nullptr, &dstrect);
}



void Renderer::Render(const Menu& menu)
{
    for (const std::unique_ptr<Widget>& widget : menu.GetWidgets())
        widget->OnRender(*this);
}



void Renderer::Render(const PerfMonitor& monitor)
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Performance Monitor");
    ImGui::Text("Frame Rate: %u FPS", monitor.GetFrameRate());
    ImGui::Text("Frame Time: %u ms",  monitor.GetFrameTime());
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_rendererContext);
}



void Renderer::Display()
{
    SDL_RenderPresent(m_rendererContext);
}



Renderer::~Renderer()
{
    GAME_2D_LOG_DEBUG("Denitializing ImGui SDL3 renderer backend\n");
    ImGui_ImplSDLRenderer3_Shutdown();

    GAME_2D_LOG_DEBUG("Denitializing ImGui SDL3 backend\n");
    ImGui_ImplSDL3_Shutdown();

    GAME_2D_LOG_DEBUG("Denitializing ImGui context\n");
    ImGui::DestroyContext();

    GAME_2D_LOG_DEBUG("Denitializing SDL3 renderer context\n");
    SDL_DestroyRenderer(m_rendererContext);
}
