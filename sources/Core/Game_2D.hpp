#pragma once

#include <box2d/b2_math.h>                          // b2Vec2 class
#include <SDL3/SDL_events.h>                        // SDL_Event union

#include <memory>                                   // std::unique_ptr
#include <utility>                                  // std::pair
#include <vector>                                   // std::vector

#include "Collision/Contact_Listener.hpp"           // ContactListener class
#include "Entity/Enemy.hpp"                         // Enemy class
#include "Entity/Game_Object.hpp"                   // GameObject class
#include "Entity/Player.hpp"                        // Player class
#include "Game_State/Game_State.hpp"                // GAME_STATE_TYPE enum
#include "Game_State/Game_State_Manager.hpp"        // GameStateManager class
#include "Media/Asset.hpp"                          // AssetManager class
#include "Media/Audio.hpp"                          // Audio class
#include "Media/Camera.hpp"                         // Camera class
#include "Media/Renderer.hpp"                       // Renderer class
#include "Media/Sprite.hpp"                         // Sprite class
#include "Media/Window.hpp"                         // Window class
#include "UI/Menu.hpp"                              // Menu class
#include "Utility/Perf_Monitor.hpp"                 // PerfMonitor class


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

    // Creates the game menus
    void CreateMenus();

    // Creates the game object instances
    void CreateObjects();

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

private:
    // Windows and Widgets
    // ===================

    // Game window
    Window m_window;

    // Game renderer
    Renderer m_renderer;

    // The main menu
    Menu m_mainMenu;

    // The settings menu
    Menu m_settingsMenu;

    // The pause menu
    Menu m_pauseMenu;

    // The in-game level UI
    Menu m_levelUI;



    // Game State Management
    // =====================

    // Finite state machine that manages the current state of the game
    GameStateManager m_stateManager;



    // Assets
    // ======

    // Manages the Audio, Font, and Texture assets.
    AssetManager m_assetManager;

    // Music soundtrack for game
    Audio m_soundTrack;



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
