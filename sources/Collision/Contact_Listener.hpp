#pragma once

#include <box2d/b2_world_callbacks.h>   // b2ContactListener class, b2ContactImpulse class

class  b2Contact;
class  b2Fixture;
struct b2Manifold;

class Player;


/**
 *  Class responsible for detecting player-ground collisions
 */
class ContactListener final : public b2ContactListener
{
public:
    // IMPLEMENTATION
    // ==============

    // Default constructor
    ContactListener();

    // Links the Box2D contact listener to the player's ground information.
    //
    // Note: This method should be called before calling the game loop. The
    //       ground sensor should not be null.
    void LinkToPlayer(Player& player);

private:
    // CALLBACK FUNCTIONS
    // ==================

    // Override function for b2ContactListener::BeginContact
    //
    // Used for detecting when player is colliding with ground
    virtual void BeginContact(b2Contact* contact) override;

    // Override function for b2ContactListener::EndContact
    //
    // Used for deteching when player is no longer colliding with ground
    virtual void EndContact(b2Contact* contact) override;

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and assignment operator have been disabled
    //       to prevent two instances from modifying the player ground
    //       contacts count, resulting in the incorrect value.
    ContactListener(const ContactListener& listener) = delete;
    void operator=(const ContactListener& listener) = delete;

private:
    // This is required for the Box2D collision call back.
    friend class b2World;

    // Pointer to the player's ground sensor
    b2Fixture* m_sensorData;

    // Pointer to the player's groiund contact count
    uint8_t* m_contactsData;

    // Note: These variables are populated by calling the 'LinkToPlayer' method.
};