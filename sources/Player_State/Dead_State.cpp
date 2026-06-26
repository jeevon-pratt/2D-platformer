#include "Entity/Player.hpp"                // Player class
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// **************
// IMPLEMENTATION
// **************

void DeadState::OnEnter(Player& player)
{
    player.ResetAnimation("dead_idle");
}



void DeadState::OnHandle(Player& player)
{
    // No functionality
}



void DeadState::OnUpdate(Player& player)
{    
    bool isDead = ApproxEq(player.GetHealth(), 0.0f);

    if (!isDead)
        player.SetState(IDLE_STATE);

    player.PlayAnimation("dead_idle");
}



void DeadState::OnExit(Player& player)
{
    // No functionality
}
