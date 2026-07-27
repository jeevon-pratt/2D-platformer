#pragma once

#include <box2d/b2_fixture.h>                       // b2Fixture class
#include <box2d/b2_math.h>                          // b2Vec2 class

#include "Entity/Game_Object.hpp"                   // GameObject class
#include "Player_State/Player_State_Manager.hpp"    // PlayerStateManager class

class b2World;

namespace Json
{
    class Value;
}



/**
 *  Class for representing player
 */
class Player final : public GameObject
{
public:
    // SYMBOLIC CONSTANTS
    // ==================

    static constexpr float WALK_SPEED = 10.0f; // m/s
    static constexpr float JUMP_SPEED = 8.0f;  // m/s

public:
    // IMPLEMENTATION
    // ==============

    // Default constructor
    Player();

    // Move Constructor
    //
    // Note: 1) If 'player' does not already have a sprite, 'CreateSprite' will have to
    //          be called.
    //
    //       2) If 'player' does not already have a physics body defined, 'CreateBody'
    //          will have to be called.
    Player(Player&& player) noexcept;

    // Move Assignment Operator
    //
    // Note: 1) If 'player' does not already have a sprite 'CreateSprite' will have to
    //          be called.
    //
    //       2) If 'player' does not already have a physics body defined, 'CreateBody'
    //          will have to be called.
    void operator=(Player&& player) noexcept;

    // Creates the physics body, main fixture, and sensor of the player
    virtual void CreateHitBox(b2World& world, const Json::Value& player) override;

    // Returns the player's health
    float GetHealth() const;

    // Returns the player's impact speed
    float GetImpactSpeed() const;

    // Returns a boolean indicating whether the player is on the ground
    bool IsGrounded() const;

    // Retunrs a boolean indicating if 'state' is the current player state
    bool IsState(PlayerStateID state) const;

    // Manually sets the health of the player
    void SetHealth(float health);

    // Apply's damage to the player
    //
    // Note: Damage should be positive
    void ApplyDamage(float externalDamage);

    // Uppdates the player state
    void SetState(PlayerStateID state);

    // Handles user input
    void HandleInput();

    // Updates the active state
    void Update();

    // Resets player at spawn point with full health
    virtual void Respawn() override;

private:
    // ContactListener class requires access to the player ground sensor and ground
    // contacts count.
    friend class ContactListener;


    // The health of the player
    //
    // Note: The health cannont be negative nor can it be greater than
    //       the maximum health.
    float m_health;

    // The player is contacting the ground
    //
    // Note: This field is initialized upon calling 'CreateHitBox'
    b2Fixture* m_groundSensor;

    // The number of fixture contacts with the sesnsor
    //
    // Note: This field is modfied by the 'ContactListener' class
    uint8_t m_groundContacts;

    // The vertical speed of the player when the ground sensor collides with
    // another body.
    //
    // Note: This field is modified by the 'ContactListener' class
    float m_impactSpeed;

    // Finite state machine that manages the current state of the game
    PlayerStateManager m_stateManager;
};
