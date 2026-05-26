#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// **************
// IMPLEMENTATION
// **************

void JumpState::OnEnter(Player& player)
{
    // No Functionality
}



void JumpState::OnHandle(Player& player)
{    
    if (g_keyState[SCANCODE_W])
    {
        b2Vec2 currentVel = player.GetVelocity();

        // Note: The x-component of the jump velocity is not 0 to make it possible
        //       for the player to jump while walking in any direction.

        if ( player.GetGroundContactsCount() > 0 )
            player.SetVelocity( b2Vec2(currentVel.x, Player::JUMP_SPEED) );
    }

    
    if (g_keyState[SCANCODE_F])
    {
        if (player.GetGroundContactsCount() == 0)
            return;

        player.GetStateManager().PopState();
        player.GetStateManager().PushState(ATTACK_STATE);
    }

    
    if (!g_keyState[SCANCODE_A]
        && !g_keyState[SCANCODE_D]
        && !g_keyState[SCANCODE_W]
        && !g_keyState[SCANCODE_F])
    {
        if (player.GetGroundContactsCount() == 0)
            return;

        player.GetStateManager().PopState();
        player.GetStateManager().PushState(IDLE_STATE);
    }
}



void JumpState::OnUpdate(Player& player)
{
    if ( ApproxEq(player.GetHealth(), 0.0f) )
    {
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(DEAD_STATE);
        return;
    }

    player.GetSprite().PlayAnimation("default"); // "jump"
    player.GetStateManager().PopState();
    player.GetStateManager().PushState(FREE_FALL_STATE);
}



void JumpState::OnExit(Player& player)
{
    // No Functionality
}
