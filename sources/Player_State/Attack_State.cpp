#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// **************
// IMPLEMENTATION
// **************

void AttackState::OnEnter(Player& player)
{
    player.ResetAnimation("attack");
}



void AttackState::OnHandle(Player& player)
{
    if (!g_keyState[SCANCODE_A]
        && !g_keyState[SCANCODE_D]
        && !g_keyState[SCANCODE_W]
        && !g_keyState[SCANCODE_F])
    {
        player.SetState(IDLE_STATE);
        return;
    }


    if (g_keyState[SCANCODE_A] || g_keyState[SCANCODE_D])
        player.SetState(WALK_STATE);


    if (g_keyState[SCANCODE_W])
        player.SetState(JUMP_STATE);
}



void AttackState::OnUpdate(Player& player)
{
    bool isDead    = ApproxEq(player.GetHealth(), 0.0f);
    bool isFalling = !player.IsGrounded() && (player.GetVelocity().y <= 0.0f);

    if (isDead)
        player.SetState(DEAD_STATE);

    else if (isFalling)
        player.SetState(FREE_FALL_STATE);

    player.PlayAnimation("attack");
}



void AttackState::OnExit(Player& player)
{
    // No functionality
}
