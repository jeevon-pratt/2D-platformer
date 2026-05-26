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

    // For attacking the player
    void Attack(Player& player);

    // Indicates if the enemy is in the attack state
    bool IsAttacking() const;

    // Determines if the enemy attack cooldown has started
    bool IsCoolDownReset() const;

    // Determines if the enemy attack cooldown has ended
    bool IsCoolDownExpired() const;

    // Resets enemy at spawn point with full health
    virtual void Respawn() override;

private:
    // The maximum health of the enemy
    //
    // Note: The maximum health of the enemy cannot be mmodified.
    float m_maxHealth;

    // The health of the enemy
    //
    // Note: The health cannont be negative nor can it be greater
    //       than the maximum health.
    float m_health;
};
