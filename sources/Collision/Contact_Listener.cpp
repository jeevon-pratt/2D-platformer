#include <box2d/b2_body.h>                  // b2BodyType enum
#include <box2d/b2_collision.h>             // b2Manifold class
#include <box2d/b2_contact.h>               // b2Contact class
#include <box2d/b2_fixture.h>               // b2Fixture class
#include <box2d/b2_world.h>                 // b2World class

#include "Collision/Contact_Listener.hpp"   // ContactListener Class
#include "Entity/Player.hpp"                // Player class
#include "Utility/Assert.hpp"               // GAME_2D_ASSERT macro function


// **************
// IMPLEMENTATION
// **************

ContactListener::ContactListener():
    m_sensorData   (nullptr),
    m_contactsData (nullptr)
{
}



void ContactListener::LinkToPlayer(Player& player)
{
    m_sensorData = player.m_groundSensor;
    m_contactsData = &player.m_groundContacts;
}



// ******************
// CALLBACK FUNCTIONS
// ******************

void ContactListener::BeginContact(b2Contact* contact)
{
    GAME_2D_ASSERT(m_sensorData && m_contactsData);

    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    if (m_sensorData == fixtureA || m_sensorData == fixtureB)
        ++(*m_contactsData);
}



void ContactListener::EndContact(b2Contact* contact)
{
    GAME_2D_ASSERT(m_sensorData && m_contactsData);

    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    if (m_sensorData == fixtureA || m_sensorData == fixtureB)
        --(*m_contactsData);
}

