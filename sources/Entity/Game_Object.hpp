#pragma once

#include <box2d/b2_math.h>      // b2Vec2 class

#include "Media/Sprite.hpp"     // SpriteCreateInfo struct, Sprite class

struct SDL_Texture;


/**
 * Class for implementing game objects
 */
class GameObject
{
public:
    // IMPLEMENTATION
    // ==============

    // Default Constructor
    GameObject();

    // Copy Constructor
    //
    // Note: Due to the limitations of Box2D, shallow copies are performed for the
    //       physics body and main fixture. The 'CreateBody' and 'CreateMainFixture'
    //       methods should be called to create deep copies of these physics attributes.
    GameObject(const GameObject& object);

    // Assignment Operator
    //
    // Note: Due to the limitations of Box2D, shallow copies are performed for the
    //       physics body and main fixture. The 'CreateBody' and 'CreateMainFixture'
    //       methods should be called to create deep copies of these physics attributes.
    void operator=(const GameObject& object);

    // Creates the sprite of the game object
    virtual void CreateSprite(const SpriteCreateInfo& info);

    // Creates the physics body in the physics world
    virtual void CreateBody(b2World& world, b2BodyDef& definition);

    // Creates the main fixture for the body
    virtual void CreateMainFixture(b2FixtureDef& definition);

    // Returns a reference to the game object sprite
    virtual Sprite& GetSprite();

    // Returns a const reference to the game object sprite
    virtual const Sprite& GetSprite() const;

    // Returns the current position
    virtual b2Vec2 GetPosition() const;

    // Returns the linear velocity vector
    virtual b2Vec2 GetVelocity() const;

    // Returns the current angle
    virtual float GetAngle() const;

    // Returns the initial position of the object
    virtual b2Vec2 GetSpawnPoint() const;

    // Sets the transform of the physics body
    virtual void SetTransform(b2Vec2 newPos, float angle);

    // Sets the linear velocity vector
    virtual void SetVelocity(b2Vec2 force);

    // Applies a force to the center of mass
    virtual void ApplyForceToCenter(b2Vec2 force);

    // Negates the boolean that controls the direction of the game object
    virtual void Invert();

    // Returns the boolean that controls the direction of the game object
    virtual bool IsInverted() const;

    // Resets the object to its spawn point
    virtual void Respawn();

    // Default destructor
    virtual ~GameObject() = default;

protected:
    // The animation sprite of the game object
    //
    // Note: This value is not populated upon initialization if the default constructor
    //       is called. In that case, the 'CreateSprite' method should be called with
    //       the corresponding Sprite create info.
    Sprite m_sprite;

    // The direction that the game object is facing
    //
    // Note: A value of 'true' indicates that the object is facing left and a value of
    //       'false' indicates that the object is facing right.
    bool m_isInverted;

    // The Box2D physics body of the game object
    //
    // Note: 1) This value is not populated upon initialization. The 'CreateBody' method
    //          must be called with the corresponding Box2D physics world and body
    //          definition.
    //
    //       2) If a game entity without a physics body is desired, the Sprite class
    //          should be used instead.
    b2Body* m_body;

    // A pointer to the fixture tied to the physics body
    //
    // Note: This value is not populated upon initialization. The 'CreateMainFixture'
    //       method must be called with the corresponding Box2D physics world and
    //       fixture definition.
    b2Fixture* m_fixture;

    // The spawn point of the game object
    //
    // Note: The value is set when the physics body is created.
    b2Vec2 m_spawnPoint;
};
