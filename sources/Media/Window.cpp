#include <SDL2/SDL_error.h>         // SDL_GetError function
#include <SDL2/SDL_events.h>        // SDL_Event union
#include <SDL2/SDL_messagebox.h>    // SDL_ShowSimpleMessageBox function
#include <SDL2/SDL_mouse.h>         // SDL_ShowCursor function
#include <SDL2/SDL_render.h>        // SDL renderer functionality
#include <SDL2/SDL_video.h>         // SDL window functionality

#include <cstdlib>                  // std::exit, EXIT_FAILURE
#include <string_view>              // std::string_view

#include "Media/Window.hpp"         // Window class
#include "Utility/Log.hpp"          // GAME_2D_LOG_CRITICAL macro function


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr uint32_t WINDOW_X   = SDL_WINDOWPOS_CENTERED;
static constexpr uint32_t WINDOW_Y   = SDL_WINDOWPOS_CENTERED;
static constexpr uint32_t INIT_FLAGS = SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP;



// **************
// IMPLEMENTATION
// **************

Window::Window(std::string_view title, uint16_t width, uint16_t height):
    m_windowContext ( SDL_CreateWindow(title.data(), WINDOW_X, WINDOW_Y, width, height, INIT_FLAGS) )
{
    if (!m_windowContext)
    {
        GAME_2D_LOG_CRITICAL("%s\n\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }
}



const SDL_Window* Window::GetContext() const
{
    return m_windowContext;
}



uint16_t Window::GetWidth() const
{
    int width = 0;

    SDL_GetWindowSize(m_windowContext, &width, nullptr);

    return static_cast<uint16_t>(width);
}



uint16_t Window::GetHeight() const
{
    int height = 0;

    SDL_GetWindowSize(m_windowContext, nullptr, &height);

    return static_cast<uint16_t>(height);
}



void Window::HandleInput(const SDL_Event& event) const
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11)
    {
        bool isFullscreen = SDL_GetWindowFlags(m_windowContext) & SDL_WINDOW_FULLSCREEN;

        // Toggle fullscreen mode
        SDL_SetWindowFullscreen(m_windowContext, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
    }
}



void Window::ShowMessageBox(std::string_view title, std::string_view message) const
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             title.data(),
                             message.data(),
                             m_windowContext);
}



Window::~Window()
{
    SDL_DestroyWindow(m_windowContext);
}



// ****************
// STATIC FUNCTIONS
// ****************

void Window::ShowCursor()
{
    if (SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE)
        SDL_ShowCursor(SDL_ENABLE);
}



void Window::HideCursor()
{
    if (SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE)
        SDL_ShowCursor(SDL_DISABLE);
}
