#pragma once

#include <box2d/b2_fixture.h>                       // b2Fixture class
#include <box2d/b2_math.h>                          // b2Vec2 class

#include "Entity/Game_Object.hpp"                   // GameObject class
#include "Media/Sprite.hpp"                         // SpriteCreateInfo struct
#include "Player_State/Player_State_Manager.hpp"    // PlayerStateManager class


/**
 *  Class for representing player
 */
class Player final : public GameObject
{
public:
    // SYMBOLIC CONSTANTS
    // ==================

    static constexpr float WALK_SPEED = 10.0f;
    static constexpr float JUMP_SPEED = 8.0f;

public:
    // IMPLEMENTATION
    // ==============

    // Default constructor
    Player();

    // Copy Constructor
    //
    // Note: Due to the limitations of Box2D, shallow copies are performed for the
    //       physics body, main fixture, and ground sensor. The 'CreateBody',
    //       'CreateMainFixture', and 'CreateGroundSensor' methods should be called
    //       to create deep copies of these physics attributes.
    Player(const Player& player);

    // The Assignment Operator
    //
    // Note: Due to the limitations of Box2D, shallow copies are performed for the
    //       physics body, main fixture, and ground sensor. The 'CreateBody',
    //       'CreateMainFixture', and 'CreateGroundSensor' methods should be called
    //       to create deep copies of these physics attributes.
    void operator=(const Player& player);

    // Creates the foot sensor fixture for player jumping
    void CreateGroundSensor(b2FixtureDef& definition);

    // Returns the player's health
    float GetHealth() const;

    // Returns how many fixtures the ground sensor is contacting
    uint8_t GetGroundContactsCount() const;

    // Returns the player's accumulated fall distance
    float GetFallDistance() const;

    // Retunrs the player's current state
    PLAYER_STATE_TYPE GetCurrentState() const;

    // Returns a reference to player's state manager
    PlayerStateManager& GetStateManager();

    // Returns a const reference to the player's state manager
    const PlayerStateManager& GetStateManager() const;

    // Manually sets the health of the player
    void SetHealth(float health);

    // Apply's damage to the player
    //
    // Note: Damage should be positive
    void ApplyDamage(float externalDamage);

    // Updates the fall distance accumulated by the player
    void IncrementFallDistance();

    // Sets the fall distance variable to 0
    void ResetFallDistance();

    // Resets player at spawn point with full health
    virtual void Respawn() override;

private:
    // ContactListener class requires access to the player ground sensor.
    // and ground contacts count.
    friend class ContactListener;


    // The previous position of the player
    //
    // Note: The previous position is used for calculating the player's
    //       accumulated fall distance.
    b2Vec2 m_prevPos;

    // The maximum health of the player
    //
    // Note: The maximum health of the player can be modified (e.g
    //       When the player receives a power-up)
    float m_maxHealth;

    // The health of the player
    //
    // Note: The health cannont be negative nor can it be greater than
    //       the maximum health.
    float m_health;

    // The player's fall distance
    //
    // Note: The 'CalculateFallDistance' method must be called while the
    //       game is updating in order for the distance to be calculated.
    float m_fallDistance;

    // The player is contacting the ground
    //
    // Note: This variable is not populated upon initialization. The
    //       'CreateGroundSensor' method must be called with the
    //       corresponding Box2D fixture sensor.
    b2Fixture* m_groundSensor;

    // The number of fixture contacts with the sesnsor
    //
    // Note: If  the value is greater than 0, the ground sensor is contacting
    //       another fixture. This variable is necessary because more than
    //       one fixture can be in contact with the sensor at a given time.
    uint8_t m_groundContacts;

    // Finite state machine that manages the current state of the game
    PlayerStateManager m_stateManager;
};
