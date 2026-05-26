#include <SDL2/SDL_events.h>            // SDL_PollEvent function

#include "Core/Game_2D.hpp"             // Game2D class
#include "Game_State/Game_State.hpp"    // Game state classes and enum


// **************
// IMPLEMENTATION
// **************

void PausedState::OnEnter(Game2D& app)
{
    // Configure Pause Menu

    Window::ShowCursor();
}



void PausedState::OnHandle(Game2D& app)
{
    if (static SDL_Event s_event; SDL_WaitEvent(&s_event))
    {
        if (s_event.type == SDL_QUIT)
        {
            app.m_stateManager.PopState();
            app.m_stateManager.PushState(GAME_OVER_STATE);
            return;
        }

        app.m_window.HandleInput(s_event);
        app.m_pauseMenu.HandleInput(s_event);
    }
}



void PausedState::OnUpdate(Game2D& app)
{

    if (app.m_pauseMenu.OnClick("RESUME_BUTTON"))
    {
        app.m_stateManager.PopState();
    }

    else if (app.m_pauseMenu.OnClick("MAIN_MENU_BUTTON"))
    {
        app.m_stateManager.PopState();
        app.m_stateManager.PushState(MAIN_MENU_STATE);
    }

    else if (app.m_pauseMenu.OnClick("SETTINGS_BUTTON"))
    {
        // The 'PopState' method is not called so that the game can
        // transition to the previous state when in the settings menu.
        app.m_stateManager.PushState(SETTINGS_STATE);
    }

    else if (app.m_pauseMenu.OnClick("QUIT_BUTTON"))
    {
        app.m_stateManager.PopState();
        app.m_stateManager.PushState(QUIT_STATE);
    }
}



void PausedState::OnRender(Game2D& app)
{
    app.m_renderer.Clear();
    app.m_renderer.Render(app.m_pauseMenu);
    app.m_renderer.Display();
}



void PausedState::OnExit(Game2D& app)
{
    // No Functionality
}
