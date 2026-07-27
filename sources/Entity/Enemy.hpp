#pragma once

#include "Entity/Game_Object.hpp"   // GameObject class
#include "Entity/Player.hpp"        // Player class
#include "Media/Sprite.hpp"         // SpriteCreateInfo struct


/**
 * Class for implementing enemies
 */
class Enemy final : public GameObject
{
public:
    // SYMBOLIC CONSTANTS
    // ==================

    static constexpr float CHASE_SPEED = 10.0f;  // m/s

public:
    // IMPLEMENTATION
    // ==============

    // Default constructor
    Enemy();

    // Returns the enemy's health
    float GetHealth() const;

    // Apply's damage to the enemy
    //
    // Note: Damage should be positive
    void ApplyDamage(float externalDamage);

    // Determine if the enemy's health is zero
    bool IsDead() const;

    // For chasing the player
    void Chase(const Player& player);

    // Resets enemy at spawn point with full health
    virtual void Respawn() override;

private:
    // The health of the enemy
    //
    // Note: The health cannont be negative nor can it be greater
    //       than the maximum health.
    float m_health;
};
