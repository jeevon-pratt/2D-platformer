#pragma once

#include <box2d/b2_math.h>                          // b2Vec2 class
#include <SDL2/SDL_events.h>                        // SDL_Event union

#include <memory>                                   // std::unique_ptr
#include <utility>                                  // std::pair
#include <vector>                                   // std::vector

#include "Collision/Contact_Listener.hpp"           // ContactListener class
#include "Entity/Enemy.hpp"                         // Enemy class
#include "Entity/Game_Object.hpp"                   // GameObject class
#include "Entity/Player.hpp"                        // Player class
#include "Game_State/Game_State.hpp"                // GAME_STATE_TYPE enum
#include "Game_State/Game_State_Manager.hpp"        // GameStateManager class
#include "Media/Audio.hpp"                          // AudioManager class
#include "Media/Camera.hpp"                         // Camera class
#include "Media/Texture_Manager.hpp"                // TextureManager class
#include "Media/Renderer.hpp"                       // Renderer class
#include "Media/Sprite.hpp"                         // Sprite class
#include "Media/Text.hpp"                           // FontManager class
#include "Media/User_Interface.hpp"                 // UserInterface class
#include "Media/Window.hpp"                         // Window class
#include "Utility/Perf_Monitor.hpp"                 // PerfMonitor class

struct _TTF_Font;                                   // typedef struct TTF_Font _TTF_Font


/**
 * Main singleton class for application
 */
class Game2D final
{
public:
    // SYMBOLIC CONSTANTS
    // ==================

    // Fixed game time step (in milliseconds)
    static constexpr float TIME_STEP = (1000.0f / 60.0f);

public:
    // IMPLEMENTATION
    // ==============

    // Initializes the SDL subsystems and Logging system
    static void Init();

    // Calls the internal run function
    static void Run();

    // Cleans up the SDL subsystems and Logging system
    static void CleanUp();

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The default constructor is private and the copy constructor and assignment
    //       operator have been disabled to enforce a single instance. These static
    //       functions should be called to run the game.
    Game2D();
    Game2D(const Game2D&) = delete;
    void operator=(const Game2D&) = delete;

    // Internal application run function
    void RunInternal();

    // Loads texture, audio, and font data
    void LoadAssets();

    // Creates the main menu user interface
    void CreateMainMenu();

    // Creates the settings menu user interface
    void CreateSettingsMenu();

    // Creates the pause menu user interface
    void CreatePauseMenu();

    // Creates the level user interface
    void CreateLevelUI();

    // Creates the player instance and physics body
    void CreatePlayer();

    // Creates the game object instances and physics bodies
    void CreateObjects();

    // Creates the enemy instances and physics bodies
    void CreateEnemies();

    // Creates the foreground and background layers
    void CreateLayers();

private:
    // Friend Classes
    // ==============

    friend class NullState;
    friend class MainMenuState;
    friend class SettingsState;
    friend class PausedState;
    friend class GameOverState;
    friend class QuitState;

    friend class Level1State;
    friend class Level2State;
    friend class Level3State;
    friend class Level4State;
    friend class Level5State;



    // Windows and Widgets
    // ===================

    // Game window object
    Window m_window;

    // Game renderer object
    Renderer m_renderer;

    // The interface for the main menu
    UserInterface m_mainMenu;

    // The interface for the settings menu
    UserInterface m_settingsMenu;

    // The interface for the pause menu
    UserInterface m_pauseMenu;

    // The in-game pause UI
    UserInterface m_levelUI;



    // Game State Management
    // =====================

    // Finite state machine that manages the current state of the game
    GameStateManager m_stateManager;



    // Assets
    // ======

    // Manages the game audio.
    AudioManager m_audioManager;

    // Manages the text fonts used in the game
    FontManager m_fontManager;

    // Manages the game textures
    TextureManager m_textureManager;



    // Game Entities
    // =============

    // Represents the player
    Player m_player;

    // All of the inanimate game objects in the game
    std::vector<GameObject> m_objects;

    // All of the enemies in the game
    std::vector<Enemy> m_enemies;

    // The game world camera
    Camera m_camera;

    // The Box2D physics world
    std::unique_ptr<b2World> m_physicsWorld;

    // Manages player-ground collisions
    ContactListener m_contactListener;

    // All of the background sprites and their corresponding positions
    std::vector<std::pair<Sprite, b2Vec2>> m_backgroundLayer;

    // All of the foreground sprites and their corresponding positions
    std::vector<std::pair<Sprite, b2Vec2>> m_foregroundLayer;



    // Performance
    // ===========

    // Tracks the game frame rate and frame time
    PerfMonitor m_perfMonitor;
};
