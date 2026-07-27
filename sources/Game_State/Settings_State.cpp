#include <SDL3/SDL_events.h>            // SDL_PollEvent function

#include "Core/Game_2D.hpp"             // Game2D class
#include "Game_State/Game_State.hpp"    // Game state classes and enum
#include "UI/Widget.hpp"                // Button::Event, Slider::Event
#include "Utility/Log.hpp"              // GAME_2D_LOG_DEBUG macro function


// **************
// IMPLEMENTATION
// **************

void SettingsState::OnEnter(Game2D& game)
{
    GAME_2D_LOG_DEBUG("Entering Settings State\n");
}



void SettingsState::OnHandle(Game2D& game)
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

        case Slider::Event:
            break;
        }

        game.m_window.HandleInput(s_event);
        game.m_settingsMenu.HandleInput(s_event);
    }
}



void SettingsState::OnUpdate(Game2D& game)
{
    game.m_settingsMenu.Update();
}



void SettingsState::OnRender(Game2D& game)
{
    game.m_renderer.Clear();
    game.m_renderer.Render(game.m_settingsMenu);
    game.m_renderer.Display();
}



void SettingsState::OnExit(Game2D& game)
{
    GAME_2D_LOG_DEBUG("Exiting Settings State\n");
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void SettingsState::ProcessButtonEvents(Game2D& game, const SDL_Event& event)
{
    std::string label = static_cast<const char*>(event.user.data1);

    if (label == "back_button")
        game.m_stateManager.PopState();
}