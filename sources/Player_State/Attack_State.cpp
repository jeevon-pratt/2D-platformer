#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// **************
// IMPLEMENTATION
// **************

void AttackState::OnEnter(Player& player)
{
    // No Functionality
}



void AttackState::OnHandle(Player& player)
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
        // Perform attack logic
        player.GetSprite().PlayAnimation("default"); // "attack"
    }


    if (!g_keyState[SCANCODE_A]
        && !g_keyState[SCANCODE_D]
        && !g_keyState[SCANCODE_W]
        && !g_keyState[SCANCODE_F])
    {
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(IDLE_STATE);
    }
}



void AttackState::OnUpdate(Player& player)
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



void AttackState::OnExit(Player& player)
{
    // No functionality
}
