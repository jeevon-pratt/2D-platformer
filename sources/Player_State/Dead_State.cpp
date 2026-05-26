#include "Entity/Player.hpp"                // Player class
#include "Player_State/Player_State.hpp"    // Player state classes and enum


// **************
// IMPLEMENTATION
// **************

void DeadState::OnEnter(Player& player)
{
    // No functionality
}



void DeadState::OnHandle(Player& player)
{
    // No functionality
}



void DeadState::OnUpdate(Player& player)
{
    player.GetSprite().PlayAnimation("default"); // "dead"

    if (player.GetHealth() > 0.0f)
    {
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(IDLE_STATE);
    }
}



void DeadState::OnExit(Player& player)
{
    // No functionality
}
