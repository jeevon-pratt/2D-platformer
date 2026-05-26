#include "Entity/Player.hpp"                        // Player class
#include "Player_State/Player_State.hpp"            // Player state and PlayerStateManager classes and State enums
#include "Player_State/Player_State_Manager.hpp"    // PlayerStateManager class
#include "Utility/Assert.hpp"                       // GAME_2D_ASSERT macro function


// **************
// IMPLEMENTATION
// **************

PlayerStateManager::PlayerStateManager():
    m_playerData   (nullptr),
    m_currentState (IDLE_STATE)
{
    // Create Player State Lookup Table
    // ================================

    m_stateTable.reserve(MAX_PLAYER_STATES);

    m_stateTable.emplace( IDLE_STATE,        new IdleState      );
    m_stateTable.emplace( WALK_STATE,        new WalkState      );
    m_stateTable.emplace( JUMP_STATE,        new JumpState      );
    m_stateTable.emplace( FREE_FALL_STATE,   new FreeFallState  );
    m_stateTable.emplace( HIT_GROUND_STATE,  new HitGroundState );
    m_stateTable.emplace( ATTACK_STATE,      new AttackState    );
    m_stateTable.emplace( DEAD_STATE,        new DeadState      );

    // Set the initial player state
    // ============================

    m_stack.push(IDLE_STATE);
}



void PlayerStateManager::LinkToPlayer(Player& player)
{
    m_playerData = &player;
}



PLAYER_STATE_TYPE PlayerStateManager::GetCurrentState() const
{
    return m_currentState;
}



void PlayerStateManager::PushState(PLAYER_STATE_TYPE newState)
{
    m_stack.push(newState);
}



void PlayerStateManager::PopState()
{
    m_stack.pop();
}



void PlayerStateManager::HandleInput()
{
    GAME_2D_ASSERT(m_playerData);

    m_stateTable.at(m_currentState)
        ->OnHandle(*m_playerData);
}



void PlayerStateManager::Update()
{
    GAME_2D_ASSERT(m_playerData);

    if (m_currentState != m_stack.top())
    {
        // Exit current state
        m_stateTable.at(m_currentState)
            ->OnExit(*m_playerData);

        // Enter new state
        m_currentState = m_stack.top();

        m_stateTable.at(m_currentState)
            ->OnEnter(*m_playerData);
    }

    m_stateTable.at(m_currentState)
        ->OnUpdate(*m_playerData);
}



PlayerStateManager::~PlayerStateManager()
{
    for (auto& [type, state] : m_stateTable)
        delete state;
}
