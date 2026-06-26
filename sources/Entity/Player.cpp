#include <box2d/b2_body.h>                  // b2Body class
#include <box2d/b2_edge_shape.h>            // b2EdgeShape class
#include <box2d/b2_world.h>                 // b2World class
#include <json/value.h>                     // Json::Value class

#include <utility>                          // std::move

#include "Entity/Player.hpp"                // Player class
#include "Player_State/Player_State.hpp"    // PlayerStateID enum
#include "Utility/Assert.hpp"               // GAME_2D_ASSERT macro function
#include "Utility/Math.hpp"                 // Clamp function


// **************
// IMPLEMENTATION
// **************

Player::Player():
    GameObject       (),
    m_health         (100.0f),
    m_groundSensor   (nullptr),
    m_groundContacts (0),
    m_impactSpeed    (0.0f)
{
}



Player::Player(Player&& player) noexcept:
    GameObject       (std::move(player)),
    m_health         (player.m_health),
    m_groundSensor   (nullptr),
    m_groundContacts (0),
    m_impactSpeed    (0.0f)
{
    player.m_body    = nullptr;
    player.m_fixture = nullptr;
}



void Player::operator=(Player&& player) noexcept
{
    if (this == &player)
        return;
    
    GameObject::operator=(std::move(player));

    m_health         = player.m_health;
    m_groundSensor   = nullptr;
    m_groundContacts = 0;
    m_impactSpeed    = 0.0f;
}



void Player::CreateHitBox(b2World& world, const Json::Value& player)
{
    GameObject::CreateHitBox(world, player);

    float hx = 0.5f * player["width"].asFloat();
    float hy = 0.5f * player["height"].asFloat();

    b2Vec2 vertex1(-hx, -hy);
    b2Vec2 vertex2( hx, -hy);


    b2EdgeShape edge;
    edge.SetTwoSided(vertex1, vertex2);

    b2FixtureDef def;
    def.shape       = &edge;
    def.density     = player["density"].asFloat();
    def.friction    = player["friction"].asFloat();
    def.restitution = player["restitution"].asFloat();

    m_groundSensor = m_body->CreateFixture(&def);
}



float Player::GetHealth() const
{
    return m_health;
}



float Player::GetImpactSpeed() const
{
    return m_impactSpeed;
}



bool Player::IsGrounded() const
{
    return (m_groundContacts > 0);
}



bool Player::IsState(PlayerStateID state) const
{
    return m_stateManager.IsState(state);
}



void Player::SetHealth(float health)
{
    m_health = health;

    Clamp(m_health, 0.0f, 100.0f);
}



void Player::ApplyDamage(float externalDamage)
{
    if (externalDamage > 0)
        m_health -= externalDamage;

    if (m_health < 0.0f)
        m_health = 0.0f;
}



void Player::SetState(PlayerStateID state)
{
    m_stateManager.PopState();
    m_stateManager.PushState(state);
}



void Player::HandleInput()
{
    m_stateManager.HandleInput(*this);
}



void Player::Update()
{
    m_stateManager.Update(*this);
}



void Player::Respawn()
{
    GameObject::Respawn();

    m_health = 100.0f;
}
