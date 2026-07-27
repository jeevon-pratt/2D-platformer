#pragma once

union SDL_Event;
class Game2D;


// *************
// GAME STATE ID
// *************

/**
 * Enumeration for denoting game state types
 */
enum GameStateID
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
    virtual void OnEnter(Game2D& game) = 0;

    // Called when handling input events
    virtual void OnHandle(Game2D& game) = 0;

    // Called when updating game state elements
    virtual void OnUpdate(Game2D& game) = 0;

    // Called when rendering game state elements
    virtual void OnRender(Game2D& game) = 0;

    // Called when exiting the current game state
    virtual void OnExit(Game2D& game) = 0;

    // Default Destructor
    virtual ~GameState() = default;

protected:
    // Processes button events
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) = 0;
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

    virtual void OnEnter(Game2D& game) = 0;
    virtual void OnHandle(Game2D& game) = 0;
    virtual void OnUpdate(Game2D& game) = 0;
    virtual void OnRender(Game2D& game) = 0;
    virtual void OnExit(Game2D& game) = 0;

    virtual ~LevelState() = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) = 0;
};



/**
 * Class for implementing a basic non-level state
 */
class NonLevelState : public GameState
{
public:
    NonLevelState() = default;

    virtual void OnEnter(Game2D& game) = 0;
    virtual void OnHandle(Game2D& game) = 0;
    virtual void OnUpdate(Game2D& game) = 0;
    virtual void OnRender(Game2D& game) = 0;
    virtual void OnExit(Game2D& game) = 0;

    virtual ~NonLevelState() = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) = 0;
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

    virtual void OnEnter(Game2D& game) override {};
    virtual void OnHandle(Game2D& game) override {};
    virtual void OnUpdate(Game2D& game) override {};
    virtual void OnRender(Game2D& game) override {};
    virtual void OnExit(Game2D& game) override {};

    virtual ~NullState() override = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) override {};
};



/**
 * Class for implementing the main menu state
 */
class MainMenuState final : public NonLevelState
{
public:
    MainMenuState() = default;

    virtual void OnEnter(Game2D& game) override;
    virtual void OnHandle(Game2D& game) override;
    virtual void OnUpdate(Game2D& game) override;
    virtual void OnRender(Game2D& game) override;
    virtual void OnExit(Game2D& game) override;

    virtual ~MainMenuState() override = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) override;
};



/**
 * Class for implementing the paused state
 */
class PausedState final : public NonLevelState
{
public:
    PausedState() = default;

    virtual void OnEnter(Game2D& game) override;
    virtual void OnHandle(Game2D& game) override;
    virtual void OnUpdate(Game2D& game) override;
    virtual void OnRender(Game2D& game) override;
    virtual void OnExit(Game2D& game) override;

    virtual ~PausedState() override = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) override;
};



/**
 * Class for implementing the settings state
 */
class SettingsState final : public NonLevelState
{
public:
    SettingsState() = default;

    virtual void OnEnter(Game2D& game) override;
    virtual void OnHandle(Game2D& game) override;
    virtual void OnUpdate(Game2D& game) override;
    virtual void OnRender(Game2D& game) override;
    virtual void OnExit(Game2D& game) override;

    virtual ~SettingsState() override = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) override;
};



/**
 * Class for implementing the game over state
 */
class GameOverState final : public NonLevelState
{
public:
    GameOverState() = default;

    virtual void OnEnter(Game2D& game) override;
    virtual void OnHandle(Game2D& game) override;
    virtual void OnUpdate(Game2D& game) override;
    virtual void OnRender(Game2D& game) override;
    virtual void OnExit(Game2D& game) override;

    virtual ~GameOverState() override = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) override;
};



/**
 * Class for implementing the quit state (no functionality)
 */
class QuitState final : public NonLevelState
{
public:
    QuitState() = default;

    virtual void OnEnter(Game2D& game) override {};
    virtual void OnHandle(Game2D& game) override {};
    virtual void OnUpdate(Game2D& game) override {};
    virtual void OnRender(Game2D& game) override {};
    virtual void OnExit(Game2D& game) override {};

    virtual ~QuitState() override = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) override {};
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

    virtual void OnEnter(Game2D& game) override;
    virtual void OnHandle(Game2D& game) override;
    virtual void OnUpdate(Game2D& game) override;
    virtual void OnRender(Game2D& game) override;
    virtual void OnExit(Game2D& game) override;

    virtual ~Level1State() override = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) override;
};



/**
 * Class for implementing the Level 2 game state
 */
class Level2State final : public LevelState
{
public:
    Level2State() = default;

    virtual void OnEnter(Game2D& game) override;
    virtual void OnHandle(Game2D& game) override;
    virtual void OnUpdate(Game2D& game) override;
    virtual void OnRender(Game2D& game) override;
    virtual void OnExit(Game2D& game) override;

    virtual ~Level2State() override = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) override;
};



/**
 * Class for implementing the Level 3 game state
 */
class Level3State final : public LevelState
{
public:
    Level3State() = default;

    virtual void OnEnter(Game2D& game) override;
    virtual void OnHandle(Game2D& game) override;
    virtual void OnUpdate(Game2D& game) override;
    virtual void OnRender(Game2D& game) override;
    virtual void OnExit(Game2D& game) override;

    virtual ~Level3State() override = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) override;
};



/**
 * Class for implementing the Level 4 game state
 */
class Level4State final : public LevelState
{
public:
    Level4State() = default;

    virtual void OnEnter(Game2D& game) override;
    virtual void OnHandle(Game2D& game) override;
    virtual void OnUpdate(Game2D& game) override;
    virtual void OnRender(Game2D& game) override;
    virtual void OnExit(Game2D& game) override;

    virtual ~Level4State() override = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) override;
};



/**
 * Class for implementing the Level 5 game state
 */
class Level5State final : public LevelState
{
public:
    Level5State() = default;

    virtual void OnEnter(Game2D& game) override;
    virtual void OnHandle(Game2D& game) override;
    virtual void OnUpdate(Game2D& game) override;
    virtual void OnRender(Game2D& game) override;
    virtual void OnExit(Game2D& game) override;

    virtual ~Level5State() override = default;

private:
    virtual void ProcessButtonEvents(Game2D& game, const SDL_Event& event) override;
};
