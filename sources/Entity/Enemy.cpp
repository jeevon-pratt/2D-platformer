#include <box2d/b2_body.h>      // b2Body functionality

#include <utility>              // std::move

#include "Core/Game_2D.hpp"     // Game2D::TIME_STEP constant
#include "Entity/Enemy.hpp"     // Enemy class
#include "Utility/Assert.hpp"   // GAME_2D_ASSERT macro function
#include "Utility/Math.hpp"     // ApproxEq and InBounds functions


// **************
// IMPLEMENTATION
// **************

Enemy::Enemy():
    GameObject  (),
    m_health    (100.0f)
{
}



float Enemy::GetHealth() const
{
    return m_health;
}



void Enemy::ApplyDamage(float externalDamage)
{
    if (externalDamage > 0)
        m_health -= externalDamage;

    // To ensure that the health is not negative
    if (m_health < 0.0f)
        m_health = 0.0f;
}



bool Enemy::IsDead() const
{
    return ApproxEq(m_health, 0.0f);
}



void Enemy::Chase(const Player& player)
{
    GAME_2D_ASSERT(m_body);

    b2Vec2 velocity = (player.GetPosition() - m_body->GetPosition());

    velocity.Normalize();
    velocity *= Enemy::CHASE_SPEED;

    m_body->SetLinearVelocity(velocity);
    m_sprite.PlayAnimation("chase");
}



void Enemy::Respawn()
{
    GameObject::Respawn();

    m_health = 100.0f;
}
