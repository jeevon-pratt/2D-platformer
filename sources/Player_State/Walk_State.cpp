#include "Entity/Player.hpp"                // Player class
#include "Media/Key_State.hpp"              // g_keyState and SCANCODE enum
#include "Player_State/Player_State.hpp"    // Player state classes and enum
#include "Utility/Math.hpp"                 // ApproxEq function


// ****************
// HELPER FUNCTIONS
// ****************

static void WalkRight(Player& player)
{
    if (player.IsInverted())
        player.Invert();

    if (!player.IsGrounded())
        return;

    b2Vec2 velocity;
    velocity.x = Player::WALK_SPEED;
    velocity.y = 0.0f;

    player.SetVelocity(velocity);
}



static void WalkLeft(Player& player)
{
    if (!player.IsInverted())
        player.Invert();

    if (!player.IsGrounded())
        return;

    b2Vec2 velocity;
    velocity.x = -Player::WALK_SPEED;
    velocity.y = 0.0f;

    player.SetVelocity(velocity);
}



// **************
// IMPLEMENTATION
// **************

void WalkState::OnEnter(Player& player)
{
    player.ResetAnimation("walk");
}



void WalkState::OnHandle(Player& player)
{
    if (!g_keyState[SCANCODE_A]
        && !g_keyState[SCANCODE_D]
        && !g_keyState[SCANCODE_W]
        && !g_keyState[SCANCODE_F])
    {
        player.SetState(IDLE_STATE);
        return;
    }


    if (g_keyState[SCANCODE_A])
        WalkLeft(player);
    

    if (g_keyState[SCANCODE_D])
        WalkRight(player);


    if (g_keyState[SCANCODE_W])
        player.SetState(JUMP_STATE);


    if (g_keyState[SCANCODE_F])
        player.SetState(ATTACK_STATE);
}



void WalkState::OnUpdate(Player& player)
{
    bool isDead    = ApproxEq(player.GetHealth(), 0.0f);
    bool isFalling = !player.IsGrounded() && (player.GetVelocity().y <= 0.0f);

    if (isDead)
        player.SetState(DEAD_STATE);

    else if (isFalling)
        player.SetState(FREE_FALL_STATE);

    player.PlayAnimation("walk");
}



void WalkState::OnExit(Player& player)
{
    // No Functionality
}
