#include "Core/Game_2D.hpp"                     // Game2D class
#include "Game_State/Game_State.hpp"            // Game state classes and enum
#include "Game_State/Game_State_Manager.hpp"    // GameStateManager class
#include "Utility/Assert.hpp"                   // GAME_2D_ASSERT macro function


// **************
// IMPLEMENTATION
// **************

GameStateManager::GameStateManager():
    m_gameData     (nullptr),
    m_currentState (NULL_STATE)
{
    // Create Game State Lookup Table
    // ==============================

    m_stateTable.reserve(MAX_GAME_STATES);

    m_stateTable.emplace( NULL_STATE,      new NullState     );
    m_stateTable.emplace( MAIN_MENU_STATE, new MainMenuState );
    m_stateTable.emplace( SETTINGS_STATE,  new SettingsState );
    m_stateTable.emplace( PAUSED_STATE,    new PausedState   );
    m_stateTable.emplace( LEVEL_1_STATE,   new Level1State   );
    m_stateTable.emplace( LEVEL_2_STATE,   new Level2State   );
    m_stateTable.emplace( LEVEL_3_STATE,   new Level3State   );
    m_stateTable.emplace( LEVEL_4_STATE,   new Level4State   );
    m_stateTable.emplace( LEVEL_5_STATE,   new Level5State   );
    m_stateTable.emplace( GAME_OVER_STATE, new GameOverState );
    m_stateTable.emplace( QUIT_STATE,      new QuitState     );

    // Set the initial game state
    // ==========================

    m_stack.push(NULL_STATE);

    // Note: Setting the initial game state to the 'NULL_STATE' ensures
    //       that the 'MainMenuState::OnEnter' method is called.
}



void GameStateManager::LinkToGame(Game2D& app)
{
    m_gameData = &app;
}



GAME_STATE_TYPE GameStateManager::GetCurrentState() const
{
    return m_currentState;
}



void GameStateManager::PushState(GAME_STATE_TYPE newState)
{
    m_stack.push(newState);
}



void GameStateManager::PopState()
{
    m_stack.pop();
}



void GameStateManager::HandleEvents()
{
    GAME_2D_ASSERT(m_gameData);

    m_stateTable.at(m_currentState)
        ->OnHandle(*m_gameData);
}



void GameStateManager::Update()
{
    GAME_2D_ASSERT(m_gameData);

    if (m_currentState != m_stack.top())
    {
        // Exit current state
        m_stateTable.at(m_currentState)
            ->OnExit(*m_gameData);

        // Enter new state
        m_currentState = m_stack.top();

        m_stateTable.at(m_currentState)
            ->OnEnter(*m_gameData);
    }

    m_stateTable.at(m_currentState)
        ->OnUpdate(*m_gameData);
}



void GameStateManager::Render()
{
    GAME_2D_ASSERT(m_gameData);

    m_stateTable.at(m_currentState)
        ->OnRender(*m_gameData);
}



GameStateManager::~GameStateManager()
{
    for (auto& [type, state] : m_stateTable)
        delete state;
}
