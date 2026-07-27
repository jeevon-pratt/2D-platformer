#include <box2d/box2d.h>                // Box2D functionality
#include <SDL3/SDL_events.h>            // SDL_PollEvent function
#include <SDL3/SDL_pixels.h>            // SDL_Color struct
#include <SDL3_ttf/SDL_ttf.h>           // TTF_Font struct

#include <print>                        // std::println
#include <utility>                      // std::pair

#include "Core/Game_2D.hpp"             // Game2D class
#include "Entity/Enemy.hpp"             // Enemy class
#include "Entity/Game_Object.hpp"       // Game class
#include "Game_State/Game_State.hpp"    // Game state classes and enum
#include "UI/Widget.hpp"                // Button::Event
#include "Utility/Log.hpp"              // GAME_2D_LOG_DEBUG macro function
#include "Utility/Save_System.hpp"      // SaveFileData struct, WriteToSaveFile function


// **************
// IMPLEMENTATION
// **************

void Level1State::OnEnter(Game2D& game)
{
    GAME_2D_LOG_DEBUG("Entering Level 1 State\n");

    game.m_soundTrack = game.m_assetManager.GetAudio("GOWR_Theme");
    game.m_soundTrack.PlayLoop();
}



void Level1State::OnHandle(Game2D& game)
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



void Level1State::OnUpdate(Game2D& game)
{
    // Update Player
    // =============

    game.m_player.Update();

    bool   hitGround  = game.m_player.IsState(HIT_GROUND_STATE);
    bool   playerDead = game.m_player.IsState(DEAD_STATE);
    b2Vec2 playerPos  = game.m_player.GetPosition();

    static Audio groundHit = game.m_assetManager.GetAudio("hit_ground");
    
    if (hitGround)
        groundHit.Play();
    
    if (playerPos.y > -200.0f)
        game.m_camera.Update(playerPos);

    if (playerPos.y < -400.0f)
        game.m_player.SetHealth(0.0f);



    // Update Game Objects
    // ===================

    game.m_objects[0].SetVelocity(3.0f, 0.0f);

    for (GameObject& object : game.m_objects)
    {
        if (object.GetPosition().x > 200.0f || object.GetPosition().y < -300.0f)
            object.Respawn();
    }



    // Update Enemies
    // ==============

    for (Enemy& enemy : game.m_enemies)
    {
        b2Vec2 displacement = (playerPos - enemy.GetPosition());

        if (!enemy.IsDead() && displacement.Length() <= 10.0f)
            enemy.Chase(game.m_player);

        if (enemy.GetPosition().x > 400.0f || enemy.GetPosition().y < -400.0f)
            enemy.Respawn();
    }



    // Update Other General Game Elements
    // ==================================

    static constexpr int8_t VEL_ITR = 8;    // Number of velocity iterations during update
    static constexpr int8_t POS_ITR = 3;    // Number of position iterations during update

    // Time step is converted from milliseconds to seconds
    game.m_physicsWorld->Step(Game2D::TIME_STEP * 0.001f, VEL_ITR, POS_ITR);


    if (playerDead)
    {
        std::println("You have died :(");

        static Audio deathSound = game.m_assetManager.GetAudio("death_sound");
        deathSound.Play();

        game.m_player.Respawn();

        for (GameObject& object : game.m_objects)
            object.Respawn();

        for (Enemy& enemy : game.m_enemies)
            enemy.Respawn();
    }

    game.m_levelUI.Update();
}



void Level1State::OnRender(Game2D& game)
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



void Level1State::OnExit(Game2D& game)
{
    GAME_2D_LOG_DEBUG("Exiting Level 1 State\n");

    game.m_soundTrack.Pause();

    SaveFileData data;
    data.level        = LEVEL_1_STATE;
    data.playerPos    = game.m_player.GetPosition();
    data.playerHealth = game.m_player.GetHealth();

    WriteToSaveFile(data);
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void Level1State::ProcessButtonEvents(Game2D& game, const SDL_Event& event)
{
    std::string label = static_cast<const char*>(event.user.data1);

    if (label == "pause_button")
        game.m_stateManager.PushState(PAUSED_STATE);
}
