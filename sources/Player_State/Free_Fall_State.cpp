#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// **************
// IMPLEMENTATION
// **************

void FreeFallState::OnEnter(Player& player)
{
    // No Functionality
}



void FreeFallState::OnHandle(Player& player)
{
    if (g_keyState[SCANCODE_A])
    {
        // Allow player to invert in mid-air
        if (!player.IsInverted())
            player.Invert();
    }

    
    if (g_keyState[SCANCODE_D])
    {
        // Allow player to invert in mid-air
        if (player.IsInverted())
            player.Invert();
    }
}



void FreeFallState::OnUpdate(Player& player)
{
    if ( ApproxEq(player.GetHealth(), 0.0f) )
    {
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(DEAD_STATE);
    }

    else if ( player.GetGroundContactsCount() > 0
              && player.GetVelocity().y <= 0.0f )
    {
        // Note: When a user presses the jump key, the player enters the
        //       'FREE_FALL_STATE' but for one or two game updates, the player
        //       is still contacting the ground.
        //
        //       To prevent the player from unnecessarliy transitioning into
        //       the 'HIT_GROUND_STATE' after jumping, there is a check to
        //       ensure that the player is not currently moving upwards.

        player.GetStateManager().PopState();
        player.GetStateManager().PushState(HIT_GROUND_STATE);
    }

    else if ( player.GetGroundContactsCount() == 0
              && player.GetVelocity().y >= 0.0f )
    {
        player.GetSprite().PlayAnimation("default"); // "jump"
    }

    else if ( player.GetGroundContactsCount() == 0
              && player.GetVelocity().y < 0.0f )
    {
        player.GetSprite().PlayAnimation("default"); // "fall"
        player.IncrementFallDistance();
    }
}



void FreeFallState::OnExit(Player& player)
{
    // No Functionality
}
