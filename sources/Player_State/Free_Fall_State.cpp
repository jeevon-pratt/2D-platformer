#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// **************
// IMPLEMENTATION
// **************

void FreeFallState::OnEnter(Player& player)
{
    player.ResetAnimation("fall");
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
    bool isDead    = ApproxEq(player.GetHealth(), 0.0f);
    bool hitGround = player.IsGrounded()  && (player.GetVelocity().y <= 0.0f);

    if (isDead)
        player.SetState(DEAD_STATE);

    else if (hitGround)
        player.SetState(HIT_GROUND_STATE);

    player.PlayAnimation("fall");
}



void FreeFallState::OnExit(Player& player)
{
    // No functionality
}
