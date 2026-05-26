#include <SDL2/SDL_events.h>            // SDL_PollEvent function

#include "Core/Game_2D.hpp"             // Game2D class
#include "Game_State/Game_State.hpp"    // Game state classes and enum


// **************
// IMPLEMENTATION
// **************

void SettingsState::OnEnter(Game2D& app)
{
    // Configure Settings Menu

    Window::ShowCursor();
}



void SettingsState::OnHandle(Game2D& app)
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
        app.m_settingsMenu.HandleInput(s_event);
    }
}



void SettingsState::OnUpdate(Game2D& app)
{
    if (app.m_settingsMenu.OnClick("BACK_BUTTON"))
        app.m_stateManager.PopState();
}



void SettingsState::OnRender(Game2D& app)
{
    app.m_renderer.Clear();
    app.m_renderer.Render(app.m_settingsMenu);
    app.m_renderer.Display();
}



void SettingsState::OnExit(Game2D& app)
{
    // No Functionality
}
