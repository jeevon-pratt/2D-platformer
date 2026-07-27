#include <box2d/box2d.h>            // Box2D functionality
#include <SDL3/SDL.h>               // SDL functionality
#include <SDL3_mixer/SDL_mixer.h>   // MIX_Init and MIX_Quit functions
#include <SDL3_ttf/SDL_ttf.h>       // TTF_Init and TTF_Quit functions
#include <json/value.h>             // Json::Value class

#ifndef __GNUC__
#include <filesystem>               // std::filesystem
#endif
#include <print>                    // std::println
#include <string>                   // std::string

#include "Core/Game_2D.hpp"         // Game2D class
#include "Utility/Assert.hpp"       // GAME_2D_LOG_ASSERT macro function
#include "Utility/Log.hpp"          // InitLog, CloseLogFile and Log macro functions
#include "Utility/Math.hpp"         // ConvertToMeters function
#include "Utility/Memory.hpp"       // LoadJson function
#include "Utility/Save_System.hpp"  // OpenSaveFile and CloseSaveFile functions


// ***********************
// CONFIGURATION CONSTANTS
// ***********************

// File Paths
// ==========

#ifdef DEBUG
static constexpr const char* BINARY_DIR     = "../../../bin/debug";
#else
static constexpr const char* BINARY_DIR     = "../../../bin/release";
#endif

static constexpr const char* ASSET_CONFIG     = "../../internal/asset_config.json";
static constexpr const char* ENTITY_CONFIG    = "../../internal/entity_config.json";
static constexpr const char* MENU_CONFIG      = "../../internal/menu_config.json";
static constexpr const char* LOG_FILE_PATH    = "../../internal/log.txt";
static constexpr const char* SAVE_FILE_PATH   = "../../internal/save.json";


// Game Constants
// ==============

static constexpr uint16_t   WINDOW_WIDTH      = 1280;
static constexpr uint16_t   WINDOW_HEIGHT     = 800;
static constexpr uint8_t    MAX_OBJECTS       = 23;
static constexpr uint8_t    MAX_ENEMIES       = 50;
static constexpr uint8_t    MAX_LAYER_SPRITES = 100;
static const     b2Vec2     GRAVITY           = {0.0f, -9.81f};



// **************
// IMPLEMENTATION
// **************

Game2D::Game2D():
    m_window       ("2D_PLATFORMER", WINDOW_WIDTH, WINDOW_HEIGHT),
    m_renderer     (m_window),
    m_mainMenu     (m_window),
    m_settingsMenu (m_window),
    m_pauseMenu    (m_window),
    m_levelUI      (m_window),
    m_assetManager (m_renderer),
    m_camera       (m_window),
    m_physicsWorld (std::make_unique<b2World>(GRAVITY))
{
    m_physicsWorld->SetContactListener(&m_contactListener);

    m_objects.reserve(MAX_OBJECTS);
    m_enemies.reserve(MAX_ENEMIES);


    LoadAssets();

    CreateMenus();
    CreateObjects();
    CreateLayers();
}



void Game2D::Init()
{
    // Setting Working Directory
    // =========================

#ifndef __GNUC__
    std::filesystem::path CURRENT_FILE = __FILE__;
    std::filesystem::path WORKING_DIR  = CURRENT_FILE/BINARY_DIR;

    std::filesystem::current_path(WORKING_DIR);
#endif


    // Initialization of Log System
    // ============================

#ifdef DEBUG
    if ( !InitLog(LOG_FILE_PATH, LOG_PRIORITY_DEBUG) )
        std::println("\aERROR: {} could not be accessed", LOG_FILE_PATH);
#endif


    // Initialization of SDL3
    // ======================

    GAME_2D_LOG_DEBUG("Initializing SDL3\n");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) || !MIX_Init() || !TTF_Init())
    {
        GAME_2D_LOG_CRITICAL("%s\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }


    // Initialization of Save System
    // =============================

    GAME_2D_LOG_DEBUG("Initializing save system\n");

    if (!OpenSaveFile(SAVE_FILE_PATH))
        GAME_2D_LOG_CRITICAL("%s could not be accessed\n", SAVE_FILE_PATH);
}



void Game2D::Run()
{
    Game2D application;

    application.RunInternal();
}



void Game2D::CleanUp()
{
    GAME_2D_LOG_DEBUG("Deinitializing save system\n");
    CloseSaveFile();

    GAME_2D_LOG_DEBUG("Deinitializing SDL3\n");
    TTF_Quit();
    MIX_Quit();
    SDL_Quit();

#ifdef DEBUG
    CloseLogFile();
#endif
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void Game2D::RunInternal()
{
    float    accumulator = 0.0f;
    uint32_t currentTime = SDL_GetTicks();

    m_stateManager.PopState();
    m_stateManager.PushState(MAIN_MENU_STATE);

    while ( !m_stateManager.IsState(QUIT_STATE) )
    {
        uint32_t newTime   = SDL_GetTicks();
        uint32_t frameTime = (newTime - currentTime);

        if (frameTime > 250)
            frameTime = 250;

        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= TIME_STEP)
        {
            m_stateManager.HandleEvents(*this);
            m_stateManager.Update(*this);

            accumulator -= TIME_STEP;
        }

        m_stateManager.Render(*this);
    }
}



void Game2D::LoadAssets()
{
    Json::Value assets = LoadJson(ASSET_CONFIG);

    // Loading Audio
    // =============

    for (const Json::Value& audio : assets["audio"])
        m_assetManager.LoadAudio(audio);


    // Loading Fonts
    // =============

    for (const Json::Value& font : assets["fonts"])     
        for (const Json::Value& size : assets["font_sizes"])
            m_assetManager.LoadFont(font, size.asUInt());


    // Loading Textures
    // ================

    for (const Json::Value& texture : assets["textures"])
        m_assetManager.LoadTexture(texture);
}



void Game2D::CreateMenus()
{
    Json::Value menus = LoadJson(MENU_CONFIG);

    m_mainMenu     .CreateWidgets( m_assetManager, menus["main_menu"]     );
    m_pauseMenu    .CreateWidgets( m_assetManager, menus["pause_menu"]    );
    m_settingsMenu .CreateWidgets( m_assetManager, menus["settings_menu"] );
    m_levelUI      .CreateWidgets( m_assetManager, menus["level_ui"]      );
}



void Game2D::CreateObjects()
{
    Json::Value entities = LoadJson(ENTITY_CONFIG);


    // Player
    // ======

    m_player.CreateSprite(m_assetManager, entities["player"]);
    m_player.CreateHitBox(*m_physicsWorld, entities["player"]);


    // Game Objects
    // ============

    for (const Json::Value& object : entities["objects"])
    {
        m_objects.emplace_back();
        m_objects.back().CreateSprite(m_assetManager, object);
        m_objects.back().CreateHitBox(*m_physicsWorld, object);
    }


    // Ground Objects
    // ==============

    Json::Value ground = entities["ground"];

    for (const Json::Value& position : ground["positions"])
    {
        float xPos = position[0].asFloat();
        float yPos = position[1].asFloat();

        m_objects.emplace_back();
        m_objects.back().CreateSprite(m_assetManager, ground);
        m_objects.back().CreateHitBox(*m_physicsWorld, ground);
        m_objects.back().SetSpawnPoint(xPos, yPos);
        m_objects.back().SetPosition(xPos, yPos);
    }

    GAME_2D_ASSERT(m_objects.size() <= MAX_OBJECTS);


    // Enemies
    // =======

    Json::Value enemy = entities["obunga"];

    for (const Json::Value& position : enemy["positions"])
    {
        float xPos = position[0].asFloat();
        float yPos = position[1].asFloat();

        m_enemies.emplace_back();
        m_enemies.back().CreateSprite(m_assetManager, enemy);
        m_enemies.back().CreateHitBox(*m_physicsWorld, enemy);
        m_enemies.back().SetSpawnPoint(xPos, yPos);
        m_enemies.back().SetPosition(xPos, yPos);
    }

    GAME_2D_ASSERT(m_objects.size() <= MAX_ENEMIES);
}



void Game2D::CreateLayers()
{
    // Create Background Layer
    m_backgroundLayer.reserve(MAX_LAYER_SPRITES);

    SpriteCreateInfo deathStarInfo;
    deathStarInfo.texture     = m_assetManager.GetTexture("death_star");
    deathStarInfo.animation   = "../../assets/dev/death_star.json";
    deathStarInfo.screenCoord = false;
    deathStarInfo.scrollFactor   = 0.01f;

    m_backgroundLayer.emplace_back(deathStarInfo, b2Vec2(7.0f, -1.5f));

    for (auto& [sprite, pos] : m_backgroundLayer)
        sprite.SetAlphaMod(100);


    // Create Foreground Layer
    m_foregroundLayer.reserve(MAX_LAYER_SPRITES);
}
