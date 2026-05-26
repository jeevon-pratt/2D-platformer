#pragma once

class Player;


// ******************
// PLAYER STATE TYPES
// ******************

/**
 * Enumeration for denoting player state types
 */
enum PLAYER_STATE_TYPE
{
    IDLE_STATE,
    WALK_STATE,
    JUMP_STATE,
    FREE_FALL_STATE,
    HIT_GROUND_STATE,
    ATTACK_STATE,
    DEAD_STATE,

    MAX_PLAYER_STATES
};


// ***********************
// BASE PLAYER STATE CLASS
// ***********************

/**
 * Interface class for player state classes
 */
class PlayerState
{
public:
    PlayerState() = default;

    virtual void OnEnter(Player& player) = 0;
    virtual void OnHandle(Player& player) = 0;
    virtual void OnUpdate(Player& player) = 0;
    virtual void OnExit(Player& player) = 0;

    virtual ~PlayerState() = default;
};


// ************************************
// DERIVED FROM BASE PLAYER STATE CLASS
// ************************************


/**
 * Class for implementing the player idle state
 */
class IdleState final : public PlayerState
{
public:
    IdleState() = default;

    virtual void OnEnter(Player& player) override;
    virtual void OnHandle(Player& player) override;
    virtual void OnUpdate(Player& player) override;
    virtual void OnExit(Player& player) override;

    virtual ~IdleState() = default;
};


/**
 * Class for implementing the walk player state
 */
class WalkState final : public PlayerState
{
public:
    WalkState() = default;

    virtual void OnEnter(Player& player) override;
    virtual void OnHandle(Player& player) override;
    virtual void OnUpdate(Player& player) override;
    virtual void OnExit(Player& player) override;

    virtual ~WalkState() = default;
};


/**
 * Class for implementing the player jump state
 */
class JumpState final : public PlayerState
{
public:
    JumpState() = default;

    virtual void OnEnter(Player& player) override;
    virtual void OnHandle(Player& player) override;
    virtual void OnUpdate(Player& player) override;
    virtual void OnExit(Player& player) override;

    virtual ~JumpState() = default;
};


/**
 * Implementation of player free fall state
 */
class FreeFallState final : public PlayerState
{
public:
    FreeFallState() = default;

    virtual void OnEnter(Player& player) override;
    virtual void OnHandle(Player& player) override;
    virtual void OnUpdate(Player& player) override;
    virtual void OnExit(Player& player) override;

    virtual ~FreeFallState() = default;
};


/**
 * Implementation of player ground impact state
 */
class HitGroundState final : public PlayerState
{
public:
    HitGroundState() = default;

    virtual void OnEnter(Player& player) override;
    virtual void OnHandle(Player& player) override;
    virtual void OnUpdate(Player& player) override;
    virtual void OnExit(Player& player) override;

    virtual ~HitGroundState() = default;
};


/**
 * Class for implementing the player attack state
 */
class AttackState final : public PlayerState
{
public:
    AttackState() = default;

    virtual void OnEnter(Player& player) override;
    virtual void OnHandle(Player& player) override;
    virtual void OnUpdate(Player& player) override;
    virtual void OnExit(Player& player) override;

    virtual ~AttackState() = default;
};


/**
 * Class for implementing the player attack state
 */
class DeadState final : public PlayerState
{
public:
    DeadState() = default;

    virtual void OnEnter(Player& player) override;
    virtual void OnHandle(Player& player) override;
    virtual void OnUpdate(Player& player) override;
    virtual void OnExit(Player& player) override;

    virtual ~DeadState() = default;
};
