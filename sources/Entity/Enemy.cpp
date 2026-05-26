#include <box2d/b2_body.h>      // b2Body functionality

#include "Core/Game_2D.hpp"     // Game2D::TIME_STEP constant
#include "Entity/Enemy.hpp"     // Enemy class
#include "Utility/Assert.hpp"   // GAME_2D_ASSERT macro function
#include "Utility/Math.hpp"     // ApproxEq and InBounds functions


// **************
// IMPLEMENTATION
// **************

Enemy::Enemy():
    GameObject  (),
    m_maxHealth (100.0f),
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
    velocity *= 10.0f;

    m_body->SetLinearVelocity(velocity);
    //sprite.PlayAnimation("chase");
}



void Enemy::Attack(Player& player)
{
    /*
    if (m_coolDownTimer == 0.0f)
    {
        player.ApplyDamage(damage);

        m_isAttacking = true;
        m_coolDownTimer += Game2D::TIME_STEP;

        //sprite.PlayAnimation("attack");
    }
    else if (m_coolDownTimer >= 1000.0f)
    {
        m_coolDownTimer = 0.0f;
        m_isAttacking = false;
    }
    else if ( InBounds(m_coolDownTimer, 0.0f, 1000.0f) )
    {
        m_coolDownTimer += Game2D::TIME_STEP;
        // sprite.PlayAnimation("attack");
    }
    */
}



bool Enemy::IsAttacking() const
{
    return false;
}



bool Enemy::IsCoolDownReset() const
{
    return false;
}



bool Enemy::IsCoolDownExpired() const
{
    return false;
}



void Enemy::Respawn()
{
    GAME_2D_ASSERT(m_body);

    m_body->SetTransform(m_spawnPoint, m_body->GetAngle());
    m_body->SetLinearVelocity( b2Vec2(0.0f, 0.0f) );

    m_health = m_maxHealth;
}
