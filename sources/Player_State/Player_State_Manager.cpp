#include "Entity/Player.hpp"                        // Player class
#include "Player_State/Player_State.hpp"            // Player state, PlayerStateManager classes and State enums
#include "Player_State/Player_State_Manager.hpp"    // PlayerStateManager class
#include "Utility/Assert.hpp"                       // GAME_2D_ASSERT macro function


// **************
// IMPLEMENTATION
// **************

PlayerStateManager::PlayerStateManager():
    m_state (IDLE_STATE)
{
    // Create Player State Lookup Table
    // ================================

    m_stateTable.reserve(MAX_PLAYER_STATES);

    m_stateTable.emplace( IDLE_STATE,        std::make_shared<IdleState>()      );
    m_stateTable.emplace( WALK_STATE,        std::make_shared<WalkState>()      );
    m_stateTable.emplace( JUMP_STATE,        std::make_shared<JumpState>()      );
    m_stateTable.emplace( FREE_FALL_STATE,   std::make_shared<FreeFallState>()  );
    m_stateTable.emplace( HIT_GROUND_STATE,  std::make_shared<HitGroundState>() );
    m_stateTable.emplace( ATTACK_STATE,      std::make_shared<AttackState>()    );
    m_stateTable.emplace( DEAD_STATE,        std::make_shared<DeadState>()      );

    // Set the initial player state
    // ============================

    m_stack.push(IDLE_STATE);
}



bool PlayerStateManager::IsState(PlayerStateID state) const
{
    return (m_state == state);
}



void PlayerStateManager::PushState(PlayerStateID newState)
{
    m_stack.push(newState);
}



void PlayerStateManager::PopState()
{
    GAME_2D_ASSERT(!m_stack.empty());
    
    m_stack.pop();
}



void PlayerStateManager::HandleInput(Player& player)
{
    GetCurrentState()->OnHandle(player);
}



void PlayerStateManager::Update(Player& player)
{
    if (m_state != m_stack.top())
    {
        // Exit current state
        GetCurrentState()->OnExit(player);

        // Enter new state
        m_state = m_stack.top();

        GetCurrentState()->OnEnter(player);
    }

    GetCurrentState()->OnUpdate(player);
}



// ******************
// INTERNAL FUNCTIONS
// ******************

std::shared_ptr<PlayerState> PlayerStateManager::GetCurrentState()
{
    return m_stateTable.at( m_state );
}
