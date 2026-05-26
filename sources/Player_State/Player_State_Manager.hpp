#pragma once

#include <stack>                                // std::stack
#include <unordered_map>                        // std::unordered_map

#include "Player_State/Player_State.hpp"        // Player state classes and enum


/**
 * Finite state machine for implementing player functionality
 */
class PlayerStateManager final
{
public:
    // IMPLEMENTATION
    // ==============

    // Default Constructor
    PlayerStateManager();

    // Links the player to the state manager
    void LinkToPlayer(Player& player);

    // Returns the current state type
    PLAYER_STATE_TYPE GetCurrentState() const;

    // Pushes a new state to the state manager stack
    void PushState(PLAYER_STATE_TYPE newState);

    // Pops the top state from the state manager stack
    void PopState();

    // Handles user input
    void HandleInput();

    // Updates the active state
    void Update();

    // Frees dynamically allocated state objects
    ~PlayerStateManager();

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and assignment operator are disabled to prevent
    //       two instances from managing the player state.
    PlayerStateManager(PlayerStateManager& manager) = delete;
    void operator=(PlayerStateManager& manager) = delete;

private:
    // A pointer to the player
    //
    // Note: This variable must be populated by calling the 'LinkToPlayer'
    //       method before running the game loop.
    Player* m_playerData;

    // The type of the current state
    PLAYER_STATE_TYPE m_currentState;

    // The stack used for transitioning between different player states
    std::stack<PLAYER_STATE_TYPE> m_stack;

    // Hash lookup table for player state objects
    std::unordered_map<PLAYER_STATE_TYPE, PlayerState*> m_stateTable;
};
