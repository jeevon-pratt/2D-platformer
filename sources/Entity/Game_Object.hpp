#pragma once

#include <box2d/b2_math.h>         // b2Vec2 class

#include "Entity/Sprite.hpp"       // Sprite class

struct SDL_Texture;
class  b2Body;
class  b2Fixture;
class  b2World;
class  TextureManager;

namespace Json
{
    class Value;
}



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

    // Move Constructor
    //
    // Note: 1) If 'object' does not already have a sprite, 'CreateSprite' will have to
    //          be called.
    //
    //       2) If 'object' does not already have a physics body defined, 'CreateBody'
    //          will have to be called.
    GameObject(GameObject&& object) noexcept;

    // Move Assignment Operator
    //
    // Note: 1) If 'object' does not already have a sprite, 'CreateSprite' will have to
    //          be called.
    //
    //       2) If 'object' does not already have a physics body defined, 'CreateBody'
    //          will have to be called.     
    void operator=(GameObject&& object) noexcept;

    // Creates the sprite of the game object
    virtual void CreateSprite(const TextureManager& manager, const Json::Value& sprite);

    // Creates the physics body and main fixture of the game object
    virtual void CreateHitBox(b2World& world, const Json::Value& object);

    // Returns a reference to the game object sprite
    virtual Sprite& GetSprite();

    // Returns a const reference to the game object sprite
    virtual const Sprite& GetSprite() const;

        // Returns the current animation frame of the sprite
    const SDL_FRect& GetSourceRect() const;

    // Returns texture of the sprite
    const SDL_Texture* GetTexture() const;

    // Returns sprite frame width in pixels
    virtual float GetFrameWidth() const;

    // Returns sprite frame height in pixels
    virtual float GetFrameHeight() const;

    // Returns the scroll factor of the sprite
    virtual float GetScrollFactor() const;

    // Returns the current position
    virtual b2Vec2 GetPosition() const;

    // Returns the linear velocity vector
    virtual b2Vec2 GetVelocity() const;

    // Returns the current angle
    virtual float GetAngle() const;

    // Returns the initial position of the object
    virtual b2Vec2 GetSpawnPoint() const;

    // Sets the angle of the physics body
    virtual void SetAngle(float angle);

    // Sets the spawn point of the physics body
    virtual void SetSpawnPoint(b2Vec2 pos);

    // Sets the position of the physics body
    virtual void SetPosition(b2Vec2 newPos);

    // Sets the transform of the physics body
    virtual void SetTransform(b2Vec2 newPos, float angle);

    // Sets the linear velocity vector
    virtual void SetVelocity(b2Vec2 force);

    // Negates the boolean that controls the direction of the game object
    virtual void Invert();

    // Returns the boolean that controls the direction of the game object
    virtual bool IsInverted() const;

    // Resets the object to its spawn point
    virtual void Respawn();

    // Resets the animation assigned to the string name
    //
    // Note: This function should be called exactly once at the moment the object
    //       transitions into a new state
    virtual void ResetAnimation(const std::string& name);

    // Plays the animation assigned to the string name
    virtual void PlayAnimation(const std::string& name);

    // Default destructor
    virtual ~GameObject() = default;

protected:
    // INTERNAL FUNCTIONS
    // ==================

    // Creates the main fixture for the object's physics body
    virtual void CreateFixture(const Json::Value& object);

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
    bool m_inverted;

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
    b2Vec2 m_spawn;
};