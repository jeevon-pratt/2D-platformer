#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// ****************
// HELPER FUNCTIONS
// ****************

static void Jump(Player& player)
{
    if (!player.IsGrounded())
        return;
   
    // Note: The x-component of the jump velocity is not 0 to make it possible
    //       for the player to jump while walking in any direction.

    b2Vec2 velocity;
    velocity.x = player.GetVelocity().x;
    velocity.y = Player::JUMP_SPEED;

    player.SetVelocity(velocity);
}



// **************
// IMPLEMENTATION
// **************

void JumpState::OnEnter(Player& player)
{
    player.ResetAnimation("jump");
}



void JumpState::OnHandle(Player& player)
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


    if (g_keyState[SCANCODE_W])
        Jump(player);

    
    if (g_keyState[SCANCODE_F])
        player.SetState(ATTACK_STATE);
}



void JumpState::OnUpdate(Player& player)
{
    bool isDead    = ApproxEq(player.GetHealth(), 0.0f);
    bool isFalling = !player.IsGrounded() && (player.GetVelocity().y <= 0.0f);
    
    if (isDead)
        player.SetState(DEAD_STATE);
       
    else if (isFalling)
        player.SetState(FREE_FALL_STATE);

    player.PlayAnimation("jump");
}



void JumpState::OnExit(Player& player)
{
    // No Functionality
}
