#include <box2d/b2_body.h>          // b2Body functionality
#include <box2d/b2_world.h>         // b2World class

#include "Entity/Game_Object.hpp"   // GameObject class
#include "Utility/Assert.hpp"       // GAME_2D_ASSERT macro function


// **************
// IMPLEMENTATION
// **************

GameObject::GameObject():
    m_isInverted (false),
    m_body       (nullptr),
    m_fixture    (nullptr),
    m_spawnPoint (0.0f, 0.0f)
{
}



GameObject::GameObject(const GameObject& object):
    m_sprite     (object.m_sprite),
    m_isInverted (false),
    m_body       (object.m_body),
    m_fixture    (object.m_fixture),
    m_spawnPoint (object.m_spawnPoint)
{
}



void GameObject::operator=(const GameObject& object)
{
    m_sprite     = object.m_sprite;
    m_isInverted = false;
    m_body       = object.m_body;
    m_fixture    = object.m_fixture;
    m_spawnPoint = object.m_spawnPoint;
}



void GameObject::CreateSprite(const SpriteCreateInfo& info)
{
    m_sprite = Sprite(info);
}



void GameObject::CreateBody(b2World& world, b2BodyDef& definition)
{
    m_body = world.CreateBody(&definition);
    m_spawnPoint = definition.position;

    m_body->SetLinearDamping(0.1f);
    m_body->SetAngularDamping(0.1f);
}



void GameObject::CreateMainFixture(b2FixtureDef& def)
{
    m_fixture = m_body->CreateFixture(&def);
}



Sprite& GameObject::GetSprite()
{
    return m_sprite;
}



const Sprite& GameObject::GetSprite() const
{
    return m_sprite;
}



b2Vec2 GameObject::GetPosition() const
{
    GAME_2D_ASSERT(m_body && m_fixture);

    return m_body->GetPosition();
}



b2Vec2 GameObject::GetVelocity() const
{
    GAME_2D_ASSERT(m_body && m_fixture);

    return m_body->GetLinearVelocity();
}



float GameObject::GetAngle() const
{
    GAME_2D_ASSERT(m_body && m_fixture);

    return m_body->GetAngle();;
}



b2Vec2 GameObject::GetSpawnPoint() const
{
    return m_spawnPoint;
}



void GameObject::SetTransform(b2Vec2 newPos, float angle)
{
    GAME_2D_ASSERT(m_body && m_fixture);

    m_body->SetTransform(newPos, angle);
}



void GameObject::SetVelocity(b2Vec2 force)
{
    GAME_2D_ASSERT(m_body && m_fixture);

    m_body->SetLinearVelocity(force);
}



void GameObject::ApplyForceToCenter(b2Vec2 force)
{
    GAME_2D_ASSERT(m_body && m_fixture);

    m_body->ApplyForceToCenter(force, true);
}



void GameObject::Invert()
{
    m_isInverted = !m_isInverted;
}



bool GameObject::IsInverted() const
{
    return m_isInverted;
}



void GameObject::Respawn()
{
    GAME_2D_ASSERT(m_body && m_fixture);

    b2Vec2 gravity = m_body->GetWorld()->GetGravity();

    m_body->SetTransform(m_spawnPoint, 0.0f);
    m_body->SetLinearVelocity( b2Vec2(0.0f, 0.0f) );
    m_body->ApplyForceToCenter(gravity, true);
}
