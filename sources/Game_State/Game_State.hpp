#pragma once

class Game2D;


// ****************
// GAME STATE TYPES
// ****************

/**
 * Enumeration for denoting game state types
 */
enum GAME_STATE_TYPE
{
    // Non-level states
    // ================

    NULL_STATE,         // Should only be used by Game State Manager
    MAIN_MENU_STATE,
    SETTINGS_STATE,
    PAUSED_STATE,
    GAME_OVER_STATE,
    QUIT_STATE,

    // Level states
    // ============

    LEVEL_1_STATE,
    LEVEL_2_STATE,
    LEVEL_3_STATE,
    LEVEL_4_STATE,
    LEVEL_5_STATE,

    MAX_GAME_STATES
};



// *********************
// BASE GAME STATE CLASS
// *********************

/**
 * Interface class for game state classes
 */
class GameState
{
public:
    // Default Constructor
    GameState() = default;

    // Called when entering a new game state
    virtual void OnEnter(Game2D& app) = 0;

    // Called when handling input events
    virtual void OnHandle(Game2D& app) = 0;

    // Called when updating game state elements
    virtual void OnUpdate(Game2D& app) = 0;

    // Called when rendering game state elements
    virtual void OnRender(Game2D& app) = 0;

    // Called when exiting the current game state
    virtual void OnExit(Game2D& app) = 0;

    // Default Destructor
    virtual ~GameState() = default;
};



// **********************************
// DERIVED FROM BASE GAME STATE CLASS
// **********************************

/**
 * Class for implementing a basic Level state
 */
class LevelState : public GameState
{
public:
    LevelState() = default;

    virtual void OnEnter(Game2D& app) = 0;
    virtual void OnHandle(Game2D& app) = 0;
    virtual void OnUpdate(Game2D& app) = 0;
    virtual void OnRender(Game2D& app) = 0;
    virtual void OnExit(Game2D& app) = 0;

    virtual ~LevelState() = default;
};



/**
 * Class for implementing a basic non-level state
 */
class NonLevelState : public GameState
{
public:
    NonLevelState() = default;

    virtual void OnEnter(Game2D& app) = 0;
    virtual void OnHandle(Game2D& app) = 0;
    virtual void OnUpdate(Game2D& app) = 0;
    virtual void OnRender(Game2D& app) = 0;
    virtual void OnExit(Game2D& app) = 0;

    virtual ~NonLevelState() = default;
};



// **********************************
// DERIVED FROM NON-LEVEL STATE CLASS
// **********************************

/**
 * Class for implementing the null state (no functionality)
 */
class NullState final : public NonLevelState
{
public:
    NullState() = default;

    virtual void OnEnter(Game2D& app) override {};
    virtual void OnHandle(Game2D& app) override {};
    virtual void OnUpdate(Game2D& app) override {};
    virtual void OnRender(Game2D& app) override {};
    virtual void OnExit(Game2D& app) override {};

    virtual ~NullState() override = default;
};



/**
 * Class for implementing the main menu state
 */
class MainMenuState final : public NonLevelState
{
public:
    MainMenuState() = default;

    virtual void OnEnter(Game2D& app) override;
    virtual void OnHandle(Game2D& app) override;
    virtual void OnUpdate(Game2D& app) override;
    virtual void OnRender(Game2D& app) override;
    virtual void OnExit(Game2D& app) override;

    virtual ~MainMenuState() override = default;
};



/**
 * Class for implementing the paused state
 */
class PausedState final : public NonLevelState
{
public:
    PausedState() = default;

    virtual void OnEnter(Game2D& app) override;
    virtual void OnHandle(Game2D& app) override;
    virtual void OnUpdate(Game2D& app) override;
    virtual void OnRender(Game2D& app) override;
    virtual void OnExit(Game2D& app) override;

    virtual ~PausedState() override = default;
};



/**
 * Class for implementing the settings state
 */
class SettingsState final : public NonLevelState
{
public:
    SettingsState() = default;

    virtual void OnEnter(Game2D& app) override;
    virtual void OnHandle(Game2D& app) override;
    virtual void OnUpdate(Game2D& app) override;
    virtual void OnRender(Game2D& app) override;
    virtual void OnExit(Game2D& app) override;

    virtual ~SettingsState() override = default;
};



/**
 * Class for implementing the game over state
 */
class GameOverState final : public NonLevelState
{
public:
    GameOverState() = default;

    virtual void OnEnter(Game2D& app) override;
    virtual void OnHandle(Game2D& app) override;
    virtual void OnUpdate(Game2D& app) override;
    virtual void OnRender(Game2D& app) override;
    virtual void OnExit(Game2D& app) override;

    virtual ~GameOverState() override = default;
};



/**
 * Class for implementing the quit state (no functionality)
 */
class QuitState final : public NonLevelState
{
public:
    QuitState() = default;

    virtual void OnEnter(Game2D& app) override {};
    virtual void OnHandle(Game2D& app) override {};
    virtual void OnUpdate(Game2D& app) override {};
    virtual void OnRender(Game2D& app) override {};
    virtual void OnExit(Game2D& app) override {};

    virtual ~QuitState() override = default;
};



// ******************************
// DERIVED FROM LEVEL-STATE CLASS
// ******************************

/**
 * Class for implementing the Level 1 game state
 */
class Level1State final : public LevelState
{
public:
    Level1State() = default;

    virtual void OnEnter(Game2D& app) override;
    virtual void OnHandle(Game2D& app) override;
    virtual void OnUpdate(Game2D& app) override;
    virtual void OnRender(Game2D& app) override;
    virtual void OnExit(Game2D& app) override;

    virtual ~Level1State() override = default;
};



/**
 * Class for implementing the Level 2 game state
 */
class Level2State final : public LevelState
{
public:
    Level2State() = default;

    virtual void OnEnter(Game2D& app) override;
    virtual void OnHandle(Game2D& app) override;
    virtual void OnUpdate(Game2D& app) override;
    virtual void OnRender(Game2D& app) override;
    virtual void OnExit(Game2D& app) override;

    virtual ~Level2State() override = default;
};



/**
 * Class for implementing the Level 3 game state
 */
class Level3State final : public LevelState
{
public:
    Level3State() = default;

    virtual void OnEnter(Game2D& app) override;
    virtual void OnHandle(Game2D& app) override;
    virtual void OnUpdate(Game2D& app) override;
    virtual void OnRender(Game2D& app) override;
    virtual void OnExit(Game2D& app) override;

    virtual ~Level3State() override = default;
};



/**
 * Class for implementing the Level 4 game state
 */
class Level4State final : public LevelState
{
public:
    Level4State() = default;

    virtual void OnEnter(Game2D& app) override;
    virtual void OnHandle(Game2D& app) override;
    virtual void OnUpdate(Game2D& app) override;
    virtual void OnRender(Game2D& app) override;
    virtual void OnExit(Game2D& app) override;

    virtual ~Level4State() override = default;
};



/**
 * Class for implementing the Level 5 game state
 */
class Level5State final : public LevelState
{
public:
    Level5State() = default;

    virtual void OnEnter(Game2D& app) override;
    virtual void OnHandle(Game2D& app) override;
    virtual void OnUpdate(Game2D& app) override;
    virtual void OnRender(Game2D& app) override;
    virtual void OnExit(Game2D& app) override;

    virtual ~Level5State() override = default;
};
