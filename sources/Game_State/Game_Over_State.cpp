#include <SDL2/SDL_timer.h>             // SDL_GetTicks function

#include "Core/Game_2D.hpp"             // Game2D class
#include "Game_State/Game_State.hpp"    // Game state classes and enum


// **************
// IMPLEMENTATION
// **************

void GameOverState::OnEnter(Game2D& app)
{
    Window::ShowCursor();
}



void GameOverState::OnHandle(Game2D& app)
{
    // No Functionality
}



void GameOverState::OnUpdate(Game2D& app)
{
    std::string message;

    message += "You played for ";
    message += std::to_string(SDL_GetTicks() * 0.001f);
    message += " seconds";

    app.m_window.ShowMessageBox("GAME OVER", message);

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
    // No Functionality
}
