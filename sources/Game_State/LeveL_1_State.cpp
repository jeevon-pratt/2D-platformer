#include <box2d/box2d.h>                // Box2D functionality
#include <SDL2/SDL_events.h>            // SDL_PollEvent function
#include <SDL2/SDL_pixels.h>            // SDL_Color struct
#include <SDL2/SDL_ttf.h>               // TTF_Font struct

#include <iostream>                     // std::cout
#include <utility>                      // std::pair

#include "Core/Game_2D.hpp"             // Game2D class
#include "Entity/Enemy.hpp"             // Enemy class
#include "Entity/Game_Object.hpp"       // Game class
#include "Game_State/Game_State.hpp"    // Game state classes and enum
#include "Media/Sprite.hpp"             // Sprite class
#include "Utility/Save_System.hpp"      // SaveFileData struct, WriteToSaveFile function


// **************
// IMPLEMENTATION
// **************

void Level1State::OnEnter(Game2D& app)
{
    Window::HideCursor();
}



void Level1State::OnHandle(Game2D& app)
{
    static SDL_Event s_event;

    while ( SDL_PollEvent(&s_event) )
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



void Level1State::OnUpdate(Game2D& app)
{
    // Update Pause Menu
    // =================

    if (app.m_levelUI.OnClick("PAUSE_BUTTON"))
    {
        // The 'PopState' method is not called so that the game can transition to
        // the previous state when in the settings menu.
        app.m_stateManager.PushState(PAUSED_STATE);
    }



    // Update Player
    // =============

    app.m_player.GetStateManager().Update();


    PLAYER_STATE_TYPE playerState = app.m_player.GetCurrentState();
    b2Vec2            playerPos   = app.m_player.GetPosition();
    uint16_t          winWidth    = app.m_window.GetWidth();
    uint16_t          winHeight   = app.m_window.GetHeight();


    if (playerState == HIT_GROUND_STATE)
        app.m_audioManager.PlayAudio("../../assets/audio/hit_ground.wav");

    if (playerPos.y > -200.0f)
        app.m_camera.Update(playerPos, winWidth, winHeight);

    if (playerPos.y < -400.0f)
        app.m_player.SetHealth(0.0f);



    // Update Game Objects
    // ===================

    app.m_objects[0].SetVelocity( b2Vec2(3.0f, 0.0f) );

    for (GameObject& object : app.m_objects)
    {
        if (object.GetPosition().x > 200.0f || object.GetPosition().y < -300.0f)
            object.Respawn();
    }



    // Update Enemies
    // ==============

    for (Enemy& enemy : app.m_enemies)
    {
        b2Vec2 displacement = (playerPos - enemy.GetPosition());

        if (!enemy.IsDead() && displacement.Length() <= 10.0f)
            enemy.Chase(app.m_player);

        if (enemy.GetPosition().x > 400.0f || enemy.GetPosition().y < -400.0f)
            enemy.Respawn();
    }



    // Update Other General Game Elements
    // ==================================

    static constexpr int8_t VEL_ITR = 8;    // Number of velocity iterations during update
    static constexpr int8_t POS_ITR = 3;    // Number of position iterations during update

    // Time step is converted from milliseconds to seconds
    app.m_physicsWorld->Step(Game2D::TIME_STEP * 0.001f, VEL_ITR, POS_ITR);


    if (playerState == DEAD_STATE)
    {
        std::cout << "You have died :(\n";

        app.m_audioManager.PlayAudio("../../assets/audio/videogame-death-sound.wav");

        app.m_player.Respawn();

        for (GameObject& object : app.m_objects)
            object.Respawn();

        for (Enemy& enemy : app.m_enemies)
            enemy.Respawn();
    }

    // app.m_audioManager.PlayAudio("../../assets/audio/8_bit_music.wav");
}



void Level1State::OnRender(Game2D& app)
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

    app.m_renderer.Render(frameRateText, b2Vec2(10.0f,  0.0f));
    app.m_renderer.Render(frameTimeText, b2Vec2(10.0f, 35.0f));


    app.m_renderer.Display();
    app.m_perfMonitor.CalculateFrameRate();
}



void Level1State::OnExit(Game2D& app)
{
    app.m_audioManager.PauseAudio("../../assets/audio/8_bit_music.wav");

    SaveFileData data;
    data.level        = LEVEL_1_STATE;
    data.playerPos    = app.m_player.GetPosition();
    data.playerHealth = app.m_player.GetHealth();

    WriteToSaveFile(data);
}
