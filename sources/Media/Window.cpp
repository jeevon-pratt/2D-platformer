#include <SDL3/SDL_error.h>         // SDL_GetError function
#include <SDL3/SDL_events.h>        // SDL_Event union
#include <SDL3/SDL_messagebox.h>    // SDL_ShowSimpleMessageBox function
#include <SDL3/SDL_mouse.h>         // SDL_ShowCursor and SDL_HideCursor functions
#include <SDL3/SDL_render.h>        // SDL renderer functionality
#include <SDL3/SDL_video.h>         // SDL window functionality

#include <cstdlib>                  // std::exit, EXIT_FAILURE
#include <string_view>              // const std::string&

#include "Media/Window.hpp"         // Window class
#include "Utility/Log.hpp"          // Log macro functions


// **************
// IMPLEMENTATION
// **************

Window::Window(const std::string& title, uint16_t width, uint16_t height):
    m_windowContext ( SDL_CreateWindow(title.data(), width, height, SDL_WINDOW_ALWAYS_ON_TOP) )
{
    GAME_2D_LOG_DEBUG("Initializing SDL3 window context\n");

    if (!m_windowContext)
    {
        GAME_2D_LOG_CRITICAL("%s\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }
}



const SDL_Window* Window::GetContext() const
{
    return m_windowContext;
}



uint16_t Window::GetWidth() const
{
    int width;

    SDL_GetWindowSize(m_windowContext, &width, nullptr);

    return static_cast<uint16_t>(width);
}



uint16_t Window::GetHeight() const
{
    int height;

    SDL_GetWindowSize(m_windowContext, nullptr, &height);

    return static_cast<uint16_t>(height);
}



void Window::HandleInput(const SDL_Event& event) const
{
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_F11)
    {
        bool isFullscreen = SDL_GetWindowFlags(m_windowContext) & SDL_WINDOW_FULLSCREEN;

        // Toggle fullscreen mode
        SDL_SetWindowFullscreen(m_windowContext, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
    }
}



void Window::ShowMessageBox(const std::string& title, const std::string& message) const
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             title.data(),
                             message.data(),
                             m_windowContext);
}



Window::~Window()
{
    GAME_2D_LOG_DEBUG("Deinitializing SDL3 window context\n");

    SDL_DestroyWindow(m_windowContext);
}



// ****************
// STATIC FUNCTIONS
// ****************

void Window::ShowCursor()
{
    SDL_ShowCursor();
}



void Window::HideCursor()
{
    SDL_HideCursor();
}
