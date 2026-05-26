#include <box2d/b2_body.h>                  // b2Body functionality
#include <box2d/b2_world.h>                 // b2World class

#include "Entity/Player.hpp"                // Player class
#include "Player_State/Player_State.hpp"    // PLAYER_STATE_TYPE enum
#include "Utility/Assert.hpp"               // GAME_2D_ASSERT macro function
#include "Utility/Math.hpp"                 // Clamp function


// **************
// IMPLEMENTATION
// **************

Player::Player():
    GameObject       (),
    m_prevPos        (0.0f, 0.0f),
    m_maxHealth      (100.0f),
    m_health         (100.0f),
    m_fallDistance   (0.0f),
    m_groundSensor   (nullptr),
    m_groundContacts (0)
{
    m_stateManager.LinkToPlayer(*this);
}



Player::Player(const Player& player):
    GameObject       (player),
    m_prevPos        (player.m_prevPos),
    m_maxHealth      (player.m_maxHealth),
    m_health         (player.m_health),
    m_fallDistance   (player.m_fallDistance),
    m_groundSensor   (nullptr),
    m_groundContacts (0)
{
    m_stateManager.LinkToPlayer(*this);
}



void Player::operator=(const Player& player)
{
    m_sprite         = player.m_sprite;
    m_isInverted     = false;
    m_body           = player.m_body;
    m_fixture        = player.m_fixture;
    m_spawnPoint     = player.m_spawnPoint;
    m_prevPos        = player.m_prevPos;
    m_maxHealth      = player.m_maxHealth;
    m_health         = player.m_health;
    m_fallDistance   = player.m_fallDistance;
    m_groundSensor   = nullptr;
    m_groundContacts = 0;
}



void Player::CreateGroundSensor(b2FixtureDef& definition)
{
    GAME_2D_ASSERT(m_body);

    // To enforce that the fixture is a sensor
    definition.isSensor = true;

    m_groundSensor = m_body->CreateFixture(&definition);
}



float Player::GetHealth() const
{
    return m_health;
}



uint8_t Player::GetGroundContactsCount() const
{
    return m_groundContacts;
}



float Player::GetFallDistance() const
{
    return m_fallDistance;
}



PLAYER_STATE_TYPE Player::GetCurrentState() const
{
    return m_stateManager.GetCurrentState();
}



PlayerStateManager& Player::GetStateManager()
{
    return m_stateManager;
}



const PlayerStateManager& Player::GetStateManager() const
{
    return m_stateManager;
}



void Player::SetHealth(float health)
{
    m_health = health;

    Clamp(m_health, 0.0f, m_maxHealth);
}



void Player::ApplyDamage(float externalDamage)
{
    if (externalDamage > 0)
        m_health -= externalDamage;

    if (m_health < 0.0f)
        m_health = 0.0f;
}



void Player::IncrementFallDistance()
{
    if (m_prevPos.y < GetPosition().y)
        return;

    float dy = (m_prevPos.y - GetPosition().y);

    m_fallDistance += dy;
    m_prevPos = GetPosition();
}



void Player::ResetFallDistance()
{
    m_fallDistance = 0.0f;
}



void Player::Respawn()
{
    GAME_2D_ASSERT(m_body && m_fixture);

    b2Vec2 gravity = m_body->GetWorld()->GetGravity();

    m_body->SetTransform(m_spawnPoint, 0.0f);
    m_body->SetLinearVelocity( b2Vec2(0.0f, 0.0f) );
    m_body->ApplyForceToCenter(gravity, true);

    m_prevPos      = m_spawnPoint;
    m_health       = m_maxHealth;
    m_fallDistance = 0.0f;
}
