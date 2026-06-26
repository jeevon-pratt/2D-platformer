#pragma once

#include <memory>                       // std::shared_ptr
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

    // Returns a boolean indicating if 'state' is the current game state
    bool IsState(GameStateID state) const;

    // Pushes a new state to the state manager stack
    void PushState(GameStateID newState);

    // Pops the top state from the state manager stack
    void PopState();

    // Handles the events of the current state in the game loop
    void HandleEvents(Game2D& game);

    // Updates the current state in the game loop
    void Update(Game2D& game);

    // Renders the current state game loop
    void Render(Game2D& game);

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and assignment operator are disabled to
    //       prevent two instances from managing the game state.
    GameStateManager(GameStateManager& manager) = delete;
    void operator=(GameStateManager& manager) = delete;

    // Helper function that the active game state object
    std::shared_ptr<GameState> GetCurrentState();

private:
    // The type of the active game state
    GameStateID m_state;

    // The stack used for transitioning between different game states
    std::stack<GameStateID> m_stack;

    // Hash lookup table for game state objects
    std::unordered_map<GameStateID, std::shared_ptr<GameState>> m_stateTable;
};