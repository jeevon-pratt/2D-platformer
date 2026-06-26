#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// **************
// IMPLEMENTATION
// **************

void IdleState::OnEnter(Player& player)
{
    player.ResetAnimation("idle");
    
    // No Functionality
}



void IdleState::OnHandle(Player& player)
{
    if (g_keyState[SCANCODE_A] || g_keyState[SCANCODE_D])
        player.SetState(WALK_STATE);

    
    if (g_keyState[SCANCODE_W])
        player.SetState(JUMP_STATE);

    
    if (g_keyState[SCANCODE_F])
        player.SetState(ATTACK_STATE);
}



void IdleState::OnUpdate(Player& player)
{
    bool isDead    = ApproxEq(player.GetHealth(), 0.0f);
    bool isFalling = !player.IsGrounded() && (player.GetVelocity().y <= 0.0f);
    
    if (isDead)
        player.SetState(DEAD_STATE);

    else if (isFalling)
        player.SetState(FREE_FALL_STATE);

    player.PlayAnimation("idle");
}



void IdleState::OnExit(Player& player)
{
    // No functionality
}
