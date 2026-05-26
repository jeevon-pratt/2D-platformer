#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// **************
// IMPLEMENTATION
// **************

void IdleState::OnEnter(Player& player)
{
    // No Functionality
}



void IdleState::OnHandle(Player& player)
{
    if (g_keyState[SCANCODE_A]
        || g_keyState[SCANCODE_D])
    {
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(WALK_STATE);
    }

    
    if (g_keyState[SCANCODE_W])
    {
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(JUMP_STATE);
    }

    
    if (g_keyState[SCANCODE_F])
    {
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(ATTACK_STATE);
    }


    if (!g_keyState[SCANCODE_A]
        && !g_keyState[SCANCODE_D]
        && !g_keyState[SCANCODE_W]
        && !g_keyState[SCANCODE_F])
    {
        player.GetSprite().PlayAnimation("idle");
    }
}



void IdleState::OnUpdate(Player& player)
{
    if ( ApproxEq(player.GetHealth(), 0.0f) )
    {
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(DEAD_STATE);
    }

    else if ( player.GetGroundContactsCount() == 0 )
    {
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(FREE_FALL_STATE);
    }
}



void IdleState::OnExit(Player& player)
{
    // No functionality
}
