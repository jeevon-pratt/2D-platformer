#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// **************
// IMPLEMENTATION
// **************

void WalkState::OnEnter(Player& player)
{
    // No Functionality
}



void WalkState::OnHandle(Player& player)
{
    if (g_keyState[SCANCODE_A])
    {
        if (!player.IsInverted())
            player.Invert();

        player.GetSprite().PlayAnimation("walk");
        player.SetVelocity( b2Vec2(-Player::WALK_SPEED, 0.0f) );
    }
    

    if (g_keyState[SCANCODE_D])
    {
        if (player.IsInverted())
            player.Invert();

        player.GetSprite().PlayAnimation("walk");
        player.SetVelocity( b2Vec2(Player::WALK_SPEED, 0.0f) );
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
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(IDLE_STATE);
    }
}



void WalkState::OnUpdate(Player& player)
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



void WalkState::OnExit(Player& player)
{
    // No Functionality
}
