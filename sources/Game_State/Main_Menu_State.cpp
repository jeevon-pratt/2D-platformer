#include <box2d/b2_world.h>             // b2World class
#include <SDL3/SDL_events.h>            // SDL_PollEvent function

#include "Core/Game_2D.hpp"             // Game2D class
#include "Game_State/Game_State.hpp"    // Game state classes and enum
#include "UI/Widget.hpp"                // Button::Event
#include "Utility/Log.hpp"              // GAME_2D_LOG_DEBUG macro function
#include "Utility/Save_System.hpp"      // SaveFileData struct, LoadFromSaveFile function


// **************
// IMPLEMENTATION
// **************

void MainMenuState::OnEnter(Game2D& game)
{
    GAME_2D_LOG_DEBUG("Entering Main Menu State\n");

    game.m_soundTrack = game.m_assetManager.GetAudio("8_bit_music");
    game.m_soundTrack.PlayLoop();
}



void MainMenuState::OnHandle(Game2D& game)
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
        game.m_mainMenu.HandleInput(s_event);
    }   
}



void MainMenuState::OnUpdate(Game2D& game)
{
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
        enemy.SetVelocity(-10.0f, 0.0f);

        if (enemy.GetPosition().x > 400.0f || enemy.GetPosition().y < -400.0f)
            enemy.Respawn();
    }



    // Update Other General Game Elements
    // ==================================

    static constexpr int8_t VEL_ITR = 8;    // Number of velocity iterations during update
    static constexpr int8_t POS_ITR = 3;    // Number of position iterations during update

    // Time step is converted from milliseconds to seconds
    game.m_physicsWorld->Step(Game2D::TIME_STEP * 0.001f, VEL_ITR, POS_ITR);

    game.m_camera.Update(0.7f, 0.0f);

    game.m_mainMenu.Update();
}



void MainMenuState::OnRender(Game2D& game)
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

    game.m_renderer.Render(game.m_mainMenu);
    game.m_renderer.Display();
}



void MainMenuState::OnExit(Game2D& game)
{
    GAME_2D_LOG_DEBUG("Exiting Main Menu State\n");

    game.m_soundTrack.Pause();

     for (GameObject& object : game.m_objects)
        object.Respawn();

    for (Enemy& enemy : game.m_enemies)
        enemy.Respawn();

    game.m_player.SetVelocity(0.0f, 0.0f);
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void MainMenuState::ProcessButtonEvents(Game2D& game, const SDL_Event& event)
{
    std::string label = static_cast<const char*>(event.user.data1);

    if (label == "play_button")
    {
        if (!SaveExists())
        {
            game.m_stateManager.PopState();
            game.m_stateManager.PushState(LEVEL_1_STATE);
            return;
        }

        SaveFileData data = LoadFromSaveFile();

        game.m_stateManager.PopState();
        game.m_stateManager.PushState(data.level);

        auto [x, y] = data.playerPos;

        game.m_player.SetHealth(data.playerHealth);
        game.m_player.SetTransform(x, y, 0.0f);
    }

    else if (label == "quit_button")
    {
        game.m_stateManager.PopState();
        game.m_stateManager.PushState(QUIT_STATE);
    }

    else if (label == "settings_button")
        game.m_stateManager.PushState(SETTINGS_STATE);
}
