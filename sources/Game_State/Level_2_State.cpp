#include <box2d/box2d.h>                // Box2D functionality
#include <SDL3/SDL_pixels.h>            // SDL_Color struct

#include "Core/Game_2D.hpp"             // Game2D class
#include "Game_State/Game_State.hpp"    // Game state classes and enum
#include "UI/Widget.hpp"                // Button::Event
#include "Utility/Save_System.hpp"      // SaveData function, WriteToSaveFile function


// **************
// IMPLEMENTATION
// **************

void Level2State::OnEnter(Game2D& game)
{
    // No Functionality
}



void Level2State::OnHandle(Game2D& game)
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
        game.m_levelUI.HandleInput(s_event);
        game.m_renderer.HandleInput(s_event);
    }

    game.m_player.HandleInput();
}



void Level2State::OnUpdate(Game2D& game)
{
    game.m_levelUI.Update();
}



void Level2State::OnRender(Game2D& game)
{
    static constexpr SDL_Color ORANGE = { 255, 117, 24 };
    static constexpr SDL_Color RED    = { 255, 0, 0 };

    b2Vec2 camTransform = game.m_camera.GetTransform();


    game.m_renderer.Clear();
    game.m_renderer.DrawGradient(ORANGE, RED, 100.0f);

    for (const auto& [sprite, pos] : game.m_backgroundLayer)
        game.m_renderer.Render(sprite, pos, camTransform);

    for (const GameObject& object : game.m_objects)
        game.m_renderer.Render(object, camTransform);

    for (const Enemy& enemy : game.m_enemies)
        game.m_renderer.Render(enemy, camTransform);

    game.m_renderer.Render(game.m_player, camTransform);

    for (const auto& [sprite, pos] : game.m_foregroundLayer)
        game.m_renderer.Render(sprite, pos, camTransform);

    game.m_renderer.Render(game.m_levelUI);
    game.m_renderer.Render(game.m_perfMonitor);
    game.m_renderer.Display();

    game.m_perfMonitor.CalculateFrameRate();
}



void Level2State::OnExit(Game2D& game)
{
    SaveFileData data;
    data.level        = LEVEL_2_STATE;
    data.playerPos    = game.m_player.GetPosition();
    data.playerHealth = game.m_player.GetHealth();

    WriteToSaveFile(data);
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void Level2State::ProcessButtonEvents(Game2D& game, const SDL_Event& event)
{
    std::string label = static_cast<const char*>(event.user.data1);

    if (label == "pause_button")
        game.m_stateManager.PushState(PAUSED_STATE);
}
