#pragma once

#include <memory>                           // std::shared_ptr
#include <stack>                            // std::stack
#include <unordered_map>                    // std::unordered_map

#include "Player_State/Player_State.hpp"    // Player state classes and enum


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

    // Returns a boolean indicating if 'state' is the current player state
    bool IsState(PlayerStateID state) const;

    // Pushes a new state to the state manager stack
    void PushState(PlayerStateID newState);

    // Pops the top state from the state manager stack
    void PopState();

    // Handles user input
    void HandleInput(Player& player);

    // Updates the active state
    void Update(Player& player);

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and assignment operator are disabled to prevent
    //       two instances from managing the player state.
    PlayerStateManager(PlayerStateManager& manager) = delete;
    void operator=(PlayerStateManager& manager) = delete;

    // Helper function that returns the current state object
    std::shared_ptr<PlayerState> GetCurrentState();

private:
    // The ID of the current state
    PlayerStateID m_state;

    // The stack used for transitioning between different player states
    std::stack<PlayerStateID> m_stack;

    // Hash lookup table for player state objects
    std::unordered_map<PlayerStateID, std::shared_ptr<PlayerState>> m_stateTable;
};
