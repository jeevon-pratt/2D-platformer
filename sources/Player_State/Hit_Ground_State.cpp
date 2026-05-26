#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// **************
// IMPLEMENTATION
// **************

void HitGroundState::OnEnter(Player& player)
{
    float fallDistance = player.GetFallDistance();

    if (fallDistance > 5.0f)
        player.ApplyDamage(fallDistance / 2.0f - 5.0f);

    player.ResetFallDistance();
    player.GetSprite().PlayAnimation("default"); // "hit_ground"
}



void HitGroundState::OnHandle(Player& player)
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
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(IDLE_STATE);
    }
}



void HitGroundState::OnUpdate(Player& player)
{
    if ( ApproxEq(player.GetHealth(), 0.0f) )
    {
        player.GetStateManager().PopState();
        player.GetStateManager().PushState(DEAD_STATE);
    }
}



void HitGroundState::OnExit(Player& player)
{
    // No functionality
}
