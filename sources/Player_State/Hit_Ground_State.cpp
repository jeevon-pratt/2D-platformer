#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// **************
// IMPLEMENTATION
// **************

void HitGroundState::OnEnter(Player& player)
{    
    float impactSpeed = player.GetImpactSpeed();

    if (impactSpeed > 20.0f)
        player.ApplyDamage(0.5f * impactSpeed);

    player.ResetAnimation("hit_ground");
}



void HitGroundState::OnHandle(Player& player)
{
    // if hit ground animation has not completed, bypass handling

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

    
    if (g_keyState[SCANCODE_F])
        player.SetState(ATTACK_STATE);
}



void HitGroundState::OnUpdate(Player& player)
{
    bool isDead = ApproxEq(player.GetHealth(), 0.0f);
    
    if (isDead)
        player.SetState(DEAD_STATE);

    player.PlayAnimation("hit_ground");
}



void HitGroundState::OnExit(Player& player)
{
    // No functionality
}
