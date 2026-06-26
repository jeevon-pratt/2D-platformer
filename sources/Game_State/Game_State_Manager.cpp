#include "Core/Game_2D.hpp"                     // Game2D class
#include "Game_State/Game_State.hpp"            // Game state classes and enum
#include "Game_State/Game_State_Manager.hpp"    // GameStateManager class
#include "Utility/Assert.hpp"                   // GAME_2D_ASSERT macro function

// **************
// IMPLEMENTATION
// **************

GameStateManager::GameStateManager():
    m_state (NULL_STATE)
{
    // Create Game State Lookup Table
    // ==============================

    m_stateTable.reserve(MAX_GAME_STATES);

    m_stateTable.emplace( NULL_STATE,      std::make_shared<NullState>()     );
    m_stateTable.emplace( MAIN_MENU_STATE, std::make_shared<MainMenuState>() );
    m_stateTable.emplace( SETTINGS_STATE,  std::make_shared<SettingsState>() );
    m_stateTable.emplace( PAUSED_STATE,    std::make_shared<PausedState>()   );
    m_stateTable.emplace( LEVEL_1_STATE,   std::make_shared<Level1State>()   );
    m_stateTable.emplace( LEVEL_2_STATE,   std::make_shared<Level2State>()   );
    m_stateTable.emplace( LEVEL_3_STATE,   std::make_shared<Level3State>()   );
    m_stateTable.emplace( LEVEL_4_STATE,   std::make_shared<Level4State>()   );
    m_stateTable.emplace( LEVEL_5_STATE,   std::make_shared<Level5State>()   );
    m_stateTable.emplace( GAME_OVER_STATE, std::make_shared<GameOverState>() );
    m_stateTable.emplace( QUIT_STATE,      std::make_shared<QuitState>()     );

    // Set the initial game state
    // ==========================

    m_stack.push(NULL_STATE);

    // Note: Setting the initial game state to the 'NULL_STATE' ensures
    //       that the 'MainMenuState::OnEnter' method is called.
}



bool GameStateManager::IsState(GameStateID state) const
{
    return (m_state == state);
}



void GameStateManager::PushState(GameStateID newState)
{
    m_stack.push(newState);
}



void GameStateManager::PopState()
{
    m_stack.pop();
}



void GameStateManager::HandleEvents(Game2D& game)
{
    GetCurrentState()->OnHandle(game);
}



void GameStateManager::Update(Game2D& game)
{
    if (m_state != m_stack.top())
    {
        // To exit current state
        GetCurrentState()->OnExit(game);

        // To enter new state
        m_state = m_stack.top();

        GetCurrentState()->OnEnter(game);
    }

    GetCurrentState()->OnUpdate(game);
}



void GameStateManager::Render(Game2D& game)
{
    GetCurrentState()->OnRender(game);
}



// ******************
// INTERNAL FUNCTIONS
// ******************

std::shared_ptr<GameState> GameStateManager::GetCurrentState()
{
    return m_stateTable.at( m_state );
}