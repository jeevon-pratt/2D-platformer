#include <box2d/b2_body.h>              // b2Body class, b2BodyDef struct, b2BodyType enum
#include <box2d/b2_circle_shape.h>      // b2CircleShape class
#include <box2d/b2_edge_shape.h>        // b2EdgeShape class
#include <box2d/b2_fixture.h>           // b2Fixture class
#include <box2d/b2_polygon_shape.h>     // b2PolygonShape class
#include <box2d/b2_world.h>             // b2World class
#include <json/value.h>                 // Json:::Value class

#include <cmath>                        // M_PI constant
#include <vector>                       // std::vector

#include "Entity/Game_Object.hpp"       // GameObject class
#include "Media/Sprite.hpp"             // SpriteCreateInfo struct
#include "Media/Texture_Manager.hpp"    // TextureManager class
#include "Utility/Assert.hpp"           // GAME_2D_ASSERT macro function
#include "Utility/Log.hpp"              // GAME_2D_LOG_ERROR macro function


// **************
// IMPLEMENTATION
// **************

GameObject::GameObject():
    m_isInverted (false),
    m_body       (nullptr),
    m_fixture    (nullptr),
    m_spawn      (0.0f, 0.0f)
{
}



GameObject::GameObject(const GameObject& object):
    m_sprite     (object.m_sprite),
    m_isInverted (false),
    m_body       (object.m_body),
    m_fixture    (object.m_fixture),
    m_spawn      (object.m_spawn)
{
}



void GameObject::operator=(const GameObject& object)
{
    m_sprite     = object.m_sprite;
    m_isInverted = false;
    m_body       = object.m_body;
    m_fixture    = object.m_fixture;
    m_spawn      = object.m_spawn;
}



void GameObject::CreateSprite(const TextureManager& manager, const Json::Value& sprite)
{
    const char* texture   = sprite["texture"].asCString();
    const char* animation = sprite["animation"].asCString();

    SpriteCreateInfo info;
    info.texture      = manager.Get(texture);
    info.animation    = animation;
    info.screenCoord  = false;
    info.scrollFactor = 1.0f;

    m_sprite = Sprite(info);
}



void GameObject::CreateHitBox(b2World& world, const Json::Value& object)
{
    float   angle = object["angle"].asFloat();
    float   xPos  = object["position"][0].asFloat();
    float   yPos  = object["position"][1].asFloat();
    uint8_t type  = object["body_type"].asUInt();

    b2BodyDef bodyDef;
    bodyDef.type          = static_cast<b2BodyType>(type);
    bodyDef.fixedRotation = object["fixed_rotation"].asBool();
    bodyDef.position      = b2Vec2(xPos, yPos);
    bodyDef.angle         = angle * static_cast<float>(M_PI / 180.0);

    m_body  = world.CreateBody(&bodyDef);
    m_spawn = b2Vec2(xPos, yPos);

    CreateFixture(object);
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
    return m_spawn;
}



void GameObject::SetAngle(float angle)
{
    GAME_2D_ASSERT(m_body && m_fixture);

    b2Vec2 pos = m_body->GetPosition();

    m_body->SetTransform(pos, angle);
}



void GameObject::SetSpawnPoint(b2Vec2 pos)
{
    m_spawn = pos;
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

    m_body->SetTransform(m_spawn, 0.0f);
    m_body->SetLinearVelocity( b2Vec2(0.0f, 0.0f) );
    m_body->ApplyForceToCenter(gravity, true);
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void GameObject::CreateFixture(const Json::Value& object)
{
    b2FixtureDef fixtureDef;
    fixtureDef.density     = object["density"].asFloat();
    fixtureDef.friction    = object["friction"].asFloat();
    fixtureDef.restitution = object["restitution"].asFloat();

    std::string shape = object["shape"].asString();

    if (shape == "box")
    {
        float hx = 0.5f * object["width"].asFloat();
        float hy = 0.5f * object["height"].asFloat();

        b2PolygonShape box;
        box.SetAsBox(hx, hy);

        fixtureDef.shape = &box;


        m_fixture = m_body->CreateFixture(&fixtureDef);
    }

    else if (shape == "ball")
    {
        b2CircleShape ball;
        ball.m_radius = object["radius"].asFloat();

        fixtureDef.shape = &ball;


        m_fixture = m_body->CreateFixture(&fixtureDef);
    }

    else if (shape == "polygon")
    {
        std::vector<b2Vec2> vertices;

        for (const Json::Value& vertex : object["vertices"])
        {
            float vx = vertex[0].asFloat();
            float vy = vertex[1].asFloat();

            vertices.emplace_back(vx, vy);
        }

        b2PolygonShape polygon;
        polygon.Set( vertices.data(), vertices.size() );

        fixtureDef.shape = &polygon;

        m_fixture = m_body->CreateFixture(&fixtureDef);
    }

    else if (shape == "edge")
    {
        float vx1 = object["vertex_1"][0].asFloat();
        float vy1 = object["vertex_1"][1].asFloat();

        float vx2 = object["vertex_2"][0].asFloat();
        float vy2 = object["vertex_2"][1].asFloat();

        b2Vec2 vertex1(vx1, vy1);
        b2Vec2 vertex2(vx2, vy2);

        b2EdgeShape edge;
        edge.SetTwoSided(vertex1, vertex2);

        fixtureDef.shape = &edge;

        m_fixture = m_body->CreateFixture(&fixtureDef);
    }

    else
        GAME_2D_LOG_ERROR("Object has an invalid shape field\n\n");
}
