#include <box2d/box2d.h>                // Box2D functionality
#include <SDL2/SDL_pixels.h>            // SDL_Color struct

#include "Core/Game_2D.hpp"             // Game2D class
#include "Game_State/Game_State.hpp"    // Game state classes and enum
#include "Utility/Save_System.hpp"      // SaveData function, WriteToSaveFile function


// **************
// IMPLEMENTATION
// **************

void Level4State::OnEnter(Game2D& app)
{
    Window::HideCursor();
}



void Level4State::OnHandle(Game2D& app)
{
    static SDL_Event s_event;

    while (SDL_PollEvent(&s_event))
    {
        if (s_event.type == SDL_QUIT)
        {
            app.m_stateManager.PopState();
            app.m_stateManager.PushState(GAME_OVER_STATE);
            return;
        }

        app.m_window.HandleInput(s_event);
        app.m_levelUI.HandleInput(s_event);
    }

    app.m_player.GetStateManager().HandleInput();
}



void Level4State::OnUpdate(Game2D& app)
{
    // Update Pause Menu
    // =================

    if (app.m_levelUI.OnClick("PAUSE_BUTTON"))
    {
        // The 'PopState' method is not called so that the game can transition to
        // the previous state when in the settings menu.
        app.m_stateManager.PushState(PAUSED_STATE);
    }
}



void Level4State::OnRender(Game2D& app)
{
    static constexpr SDL_Color s_colorOrange { 255, 117, 24 };
    static constexpr SDL_Color s_colorRed { 255, 0, 0 };

    b2Vec2 camTransform = app.m_camera.GetTransform();
    Text frameRateText = app.m_perfMonitor.GetFrameRateText();
    Text frameTimeText = app.m_perfMonitor.GetFrameTimeText();


    app.m_renderer.Clear();
    app.m_renderer.DrawGradient(s_colorOrange, s_colorRed, 100.0f);


    for (const auto& [sprite, pos] : app.m_backgroundLayer)
        app.m_renderer.Render(sprite, pos, camTransform);

    for (const GameObject& object : app.m_objects)
        app.m_renderer.Render(object, camTransform);

    for (const Enemy& enemy : app.m_enemies)
        app.m_renderer.Render(enemy, camTransform);

    app.m_renderer.Render(app.m_player, camTransform);

    for (const auto& [sprite, pos] : app.m_foregroundLayer)
        app.m_renderer.Render(sprite, pos, camTransform);

    app.m_renderer.Render(frameRateText, b2Vec2(10.0f, 0.0f));
    app.m_renderer.Render(frameTimeText, b2Vec2(10.0f, 35.0f));


    app.m_renderer.Display();
    app.m_perfMonitor.CalculateFrameRate();
}



void Level4State::OnExit(Game2D& app)
{
    SaveFileData data;
    data.level        = LEVEL_4_STATE;
    data.playerPos    = app.m_player.GetPosition();
    data.playerHealth = app.m_player.GetHealth();

    WriteToSaveFile(data);
}