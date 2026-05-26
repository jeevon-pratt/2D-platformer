#pragma once

#include <stack>                        // std::stack
#include <unordered_map>                // std::unordered_map

#include "Game_State/Game_State.hpp"    // Game state classes and enum


/**
 * Finite state machine for managing game states
 */
class GameStateManager final
{
public:
    // IMPLEMENTATION
    // ==============

    // The default constructor
    GameStateManager();

    // Links the game application instance to the state manager
    void LinkToGame(Game2D& app);

    // Returns the active game state
    GAME_STATE_TYPE GetCurrentState() const;

    // Pushes a new state to the state manager stack
    void PushState(GAME_STATE_TYPE newState);

    // Pops the top state from the state manager stack
    void PopState();

    // Handles the events of the current state in the game loop
    void HandleEvents();

    // Updates the current state in the game loop
    void Update();

    // Renders the current state game loop
    void Render();

    // Automatically destroys the dynamically allocated game state objects
    ~GameStateManager();

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and assignment operator are disabled to
    //       prevent two instances from managing the game state.
    GameStateManager(GameStateManager& manager) = delete;
    void operator=(GameStateManager& manager) = delete;

private:
    // A pointer to the game application instance
    //
    // Note: This variable must be populated by calling the 'LinkToGame'
    //       method before running the game loop.
    Game2D* m_gameData;

    // The type of the active game state
    GAME_STATE_TYPE m_currentState;

    // The stack used for transitioning between different game states
    std::stack<GAME_STATE_TYPE> m_stack;

    // Hash lookup table for game state objects
    std::unordered_map<GAME_STATE_TYPE, GameState*> m_stateTable;
};