#include <SDL3/SDL_events.h>            // SDL_PollEvent function

#include "Core/Game_2D.hpp"             // Game2D class
#include "Game_State/Game_State.hpp"    // Game state classes and enum
#include "UI/Widget.hpp"                // Button::Event
#include "Utility/Log.hpp"              // GAME_2D_LOG_DEBUG macro function


// **************
// IMPLEMENTATION
// **************

void PausedState::OnEnter(Game2D& game)
{
    GAME_2D_LOG_DEBUG("Entering Paused State\n");
}



void PausedState::OnHandle(Game2D& game)
{
    static SDL_Event s_event;
    
    while (SDL_PollEvent(&s_event))
    {
        switch (s_event.type)
        {
        case SDL_EVENT_QUIT:
            game.m_stateManager.PopState();
            game.m_stateManager.PushState(GAME_OVER_STATE);
            break;

        case Button::Event:
            ProcessButtonEvents(game, s_event);
            break;
        }

        game.m_window.HandleInput(s_event);
        game.m_pauseMenu.HandleInput(s_event);
    }
}



void PausedState::OnUpdate(Game2D& game)
{
    game.m_pauseMenu.Update();
}



void PausedState::OnRender(Game2D& game)
{
    game.m_renderer.Clear();
    game.m_renderer.Render(game.m_pauseMenu);
    game.m_renderer.Display();
}



void PausedState::OnExit(Game2D& game)
{
    GAME_2D_LOG_DEBUG("Exiting Paused State\n");
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void PausedState::ProcessButtonEvents(Game2D& game, const SDL_Event& event)
{
    std::string label = static_cast<const char*>(event.user.data1);

    if (label == "resume_button")
        game.m_stateManager.PopState();

    else if (label == "settings_button")
        game.m_stateManager.PushState(SETTINGS_STATE);

    else if (label == "main_menu_button")
    {
        game.m_stateManager.PopState();
        game.m_stateManager.PushState(MAIN_MENU_STATE);
    }
}
