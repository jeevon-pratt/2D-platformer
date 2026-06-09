#include <box2d/b2_body.h>              // b2Body class, b2BodyDef struct, b2BodyType enum
#include <box2d/b2_circle_shape.h>      // b2CircleShape class
#include <box2d/b2_fixture.h>           // b2Fixture class
#include <box2d/b2_polygon_shape.h>     // b2PolygonShape class
#include <box2d/b2_world.h>             // b2World class
#include <json/value.h>                 // Json:::Value class

#include "Entity/Game_Object.hpp"       // GameObject class
#include "Media/Sprite.hpp"             // SpriteCreateInfo struct
#include "Media/Texture_Manager.hpp"    // TextureManager class
#include "Utility/Assert.hpp"           // GAME_2D_ASSERT macro function


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



void GameObject::CreateSprite(const TextureManager& manager, const Json::Value& sprite)
{
    SpriteCreateInfo info;
    info.texture        = manager.Get( sprite["texture"].asString() );
    info.dataFilePath   = sprite["data_file"].asString();
    info.useScreenCoord = false;

    m_sprite = Sprite(info);
}



void GameObject::CreateHitBox(b2World& world, const Json::Value& object)
{
    b2BodyDef bodyDef;
    bodyDef.type          = static_cast<b2BodyType>( object["body_type"].asUInt() );
    bodyDef.fixedRotation = object["fixed_rotation"].asBool();

    m_body = world.CreateBody(&bodyDef);
    m_body->SetLinearDamping(0.1f);
    m_body->SetAngularDamping(0.1f);

    b2FixtureDef fixtureDef;
    fixtureDef.density     = object["density"].asFloat();
    fixtureDef.friction    = object["friction"].asFloat();
    fixtureDef.restitution = object["restitution"].asFloat();


    if (object["shape"].asString() == "box")
    {
        float hx = object["width" ].asFloat() / 2.0f;
        float hy = object["height"].asFloat() / 2.0f;

        b2PolygonShape boxShape;
        boxShape.SetAsBox(hx, hy);

        fixtureDef.shape = &boxShape;
        m_fixture        = m_body->CreateFixture(&fixtureDef);
    }

    else if (object["shape"].asString() == "circle")
    {
        b2CircleShape ballShape;

        ballShape.m_radius = object["radius"].asFloat();
        fixtureDef.shape   = &ballShape;
        m_fixture          = m_body->CreateFixture(&fixtureDef);
    }
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



void GameObject::SetAngle(float angle)
{
    GAME_2D_ASSERT(m_body && m_fixture);

    b2Vec2 pos = m_body->GetPosition();

    m_body->SetTransform(pos, angle);
}



void GameObject::SetSpawnPoint(b2Vec2 pos)
{
    m_spawnPoint = pos;
}



void GameObject::SetPosition(b2Vec2 newPos)
{
    GAME_2D_ASSERT(m_body && m_fixture);

    m_body->SetTransform(newPos, 0.0f);
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
