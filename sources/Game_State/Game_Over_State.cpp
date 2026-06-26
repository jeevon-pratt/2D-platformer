#include <SDL3/SDL_timer.h>             // SDL_GetTicks function

#include <format>                       // std::format

#include "Core/Game_2D.hpp"             // Game2D class
#include "Game_State/Game_State.hpp"    // Game state classes and enum
#include "Utility/Log.hpp"              // GAME_2D_LOG_DEBUG macro function



// **************
// IMPLEMENTATION
// **************

void GameOverState::OnEnter(Game2D& app)
{
    GAME_2D_LOG_DEBUG("Entering Game Over State\n");

    Window::ShowCursor();
}



void GameOverState::OnHandle(Game2D& app)
{
    // No Functionality
}



void GameOverState::OnUpdate(Game2D& app)
{
    float totalTime = SDL_GetTicks() * 0.001f;

    app.m_window.ShowMessageBox("GAME OVER",
        std::format("You played for {:.2f} seconds", totalTime));

    // Immediately quit after closing the message box
    app.m_stateManager.PopState();
    app.m_stateManager.PushState(QUIT_STATE);
}



void GameOverState::OnRender(Game2D& app)
{
    // No Functionality
}



void GameOverState::OnExit(Game2D& app)
{
    GAME_2D_LOG_DEBUG("Exiting Game Over State\n");
}
