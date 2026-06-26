#include <box2d/b2_body.h>                  // b2Body class
#include <box2d/b2_contact.h>               // b2Contact class
#include <box2d/b2_fixture.h>               // b2Fixture class
#include <box2d/b2_shape.h>                 // b2Shape::Type enum

#include "Collision/Contact_Listener.hpp"   // ContactListener Class
#include "Entity/Player.hpp"                // Player class


// ****************
// HELPER FUNCTIONS
// ****************

static bool IsSensor(const b2Fixture* fixture)
{
    if (!fixture)
        return false;

    return (fixture->GetType() == b2Shape::e_edge);
}



static Player* GetUserData(b2Fixture* fixture)
{
    if (!fixture)
        return nullptr;

    uintptr_t userData = fixture->GetBody()->GetUserData().pointer;

    return reinterpret_cast<Player*>(userData);
}



// ******************
// CALLBACK FUNCTIONS
// ******************

void ContactListener::BeginContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    if (IsSensor(fixtureA))
    {
        Player* player = GetUserData(fixtureA);

        player->m_impactSpeed = -player->GetVelocity().y;
        ++player->m_groundContacts;
    }

    else if (IsSensor(fixtureB))
    {
        Player* player = GetUserData(fixtureB);

        player->m_impactSpeed = -player->GetVelocity().y;
        ++player->m_groundContacts;
    }
}


void ContactListener::EndContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    if (IsSensor(fixtureA))
    {
        Player* player = GetUserData(fixtureA);
        --player->m_groundContacts;
    }

    else if (IsSensor(fixtureB))
    {
        Player* player = GetUserData(fixtureB);
        --player->m_groundContacts;
    }
}





