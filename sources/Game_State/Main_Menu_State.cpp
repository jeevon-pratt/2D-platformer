#include <SDL3/SDL_events.h>            // SDL_WaitEvent function

#include "Core/Game_2D.hpp"             // Game2D class
#include "Game_State/Game_State.hpp"    // Game state classes and enum
#include "Utility/Log.hpp"              // GAME_2D_LOG_DEBUG macro function
#include "Utility/Save_System.hpp"      // SaveFileData struct, LoadFromSaveFile function


// **************
// IMPLEMENTATION
// **************

static Text s_title;
static Text s_subtitle;



void MainMenuState::OnEnter(Game2D& app)
{
    GAME_2D_LOG_DEBUG("Entering Main Menu State\n");

    s_title.str    = "2D PLATFORMER";
    s_subtitle.str = "by Jee'Von Pratt";

    s_title.font    = app.m_fontManager.Get("cocogoose_48");
    s_subtitle.font = app.m_fontManager.Get("cocogoose_36");

    s_title.color.r = 0;
    s_title.color.g = 0;
    s_title.color.b = 175;

    s_subtitle.color.r = 0;
    s_subtitle.color.g = 0;
    s_subtitle.color.b = 175;
}



void MainMenuState::OnHandle(Game2D& app)
{
    if (static SDL_Event s_event; SDL_WaitEvent(&s_event))
    {
        if (s_event.type == SDL_EVENT_QUIT)
        {
            app.m_stateManager.PopState();
            app.m_stateManager.PushState(GAME_OVER_STATE);
            return;
        }

        app.m_window.HandleInput(s_event);
        app.m_mainMenu.HandleInput(s_event);
    }
}



void MainMenuState::OnUpdate(Game2D& app)
{
    if (app.m_mainMenu.OnClick("PLAY_BUTTON"))
    {
        if ( !SaveExists() )
        {
            app.m_stateManager.PopState();
            app.m_stateManager.PushState(LEVEL_1_STATE);
            return;
        }

        SaveFileData data = LoadFromSaveFile();

        app.m_stateManager.PopState();
        app.m_stateManager.PushState(data.level);

        app.m_player.SetHealth(data.playerHealth);
        app.m_player.SetTransform(data.playerPos, 0.0f);
    }


    else if (app.m_mainMenu.OnClick("SETTINGS_BUTTON"))
    {
        // The 'PopState' method is not called so that the game can transition
        // to the previous state when in the settings menu.
        app.m_stateManager.PushState(SETTINGS_STATE);
    }


    else if (app.m_mainMenu.OnClick("QUIT_BUTTON"))
    {
        app.m_stateManager.PopState();
        app.m_stateManager.PushState(QUIT_STATE);
    }
}



void MainMenuState::OnRender(Game2D& app)
{
    app.m_renderer.Clear();

    app.m_renderer.Render(app.m_mainMenu);
    app.m_renderer.Render(s_title,    b2Vec2(420.0f, 100.0f));
    app.m_renderer.Render(s_subtitle, b2Vec2(470.0f, 160.0f));

    app.m_renderer.Display();
}



void MainMenuState::OnExit(Game2D& app)
{
    GAME_2D_LOG_DEBUG("Exiting Main Menu State\n");

    // No Functionality
}
