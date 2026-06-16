#include <box2d/box2d.h>            // Box2D functionality
#include <SDL2/SDL.h>               // SDL functionality
#include <SDL2/SDL_image.h>         // IMG_GetError and IMG_Quit functions
#include <SDL2/SDL_ttf.h>           // TTF_Init and TTF_Quit functions, TTF_Font struct
#include <json/value.h>             // Json::Value class

#ifndef __GNUC__
#include <filesystem>               // std::filesystem
#endif
#include <print>                    // std::print
#include <string>                   // std::string

#include "Core/Game_2D.hpp"         // Game2D class
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
static constexpr const char* BINARY_DIR = "../../../bin/debug";
#else
static constexpr const char* BINARY_DIR = "../../../bin/release";
#endif

static constexpr const char* ASSET_CONFIG_FILE_PATH  = "../../internal/asset_config.json";
static constexpr const char* OBJECT_CONFIG_FILE_PATH = "../../internal/object_config.json";
static constexpr const char* LOG_FILE_PATH           = "../../internal/log.txt";
static constexpr const char* SAVE_FILE_PATH          = "../../internal/save.json";


// Initialization Flags
// ====================

static constexpr uint16_t SDL_FLAGS = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
static constexpr uint16_t IMG_FLAGS = IMG_INIT_PNG | IMG_INIT_JPG;


// Game Constants
// ==============

static constexpr uint16_t   WINDOW_WIDTH      = 1280;
static constexpr uint16_t   WINDOW_HEIGHT     = 800;
static constexpr uint8_t    MAX_OBJECTS       = 23;
static constexpr uint8_t    MAX_ENEMIES       = 50;
static constexpr uint8_t    MAX_LAYER_SPRITES = 100;
static const     b2Vec2     G_ACCELERATION    = b2Vec2(0.0f, -9.81f);



// **************
// IMPLEMENTATION
// **************

Game2D::Game2D():
    m_window         ("2D_PLATFORMER", WINDOW_WIDTH, WINDOW_HEIGHT),
    m_renderer       (m_window),
    m_textureManager (m_renderer),
    m_physicsWorld   ( std::make_unique<b2World>(G_ACCELERATION) ),
    m_perfMonitor    (m_renderer)
{
    m_objects.reserve(MAX_OBJECTS);
    m_enemies.reserve(MAX_ENEMIES);

    LoadAssets();

    CreateMainMenu();
    CreateSettingsMenu();
    CreatePauseMenu();
    // CreateLevelUI();
    CreateObjects();
    CreateLayers();

    m_stateManager.LinkToGame(*this);
}



void Game2D::Init()
{
    // Set the working directory
#ifndef __GNUC__
    std::filesystem::path CURRENT_FILE = __FILE__;
    std::filesystem::path WORKING_DIR  = CURRENT_FILE/BINARY_DIR;

    std::filesystem::current_path(WORKING_DIR);
#endif


    // Initialize the log system
#ifdef DEBUG
    if ( !InitLog(LOG_FILE_PATH, LOG_PRIORITY_DEBUG) )
        std::print("\aERROR: %s could not be accessed\n", LOG_FILE_PATH);
#endif


    // Initialize the save system
    if ( !OpenSaveFile(SAVE_FILE_PATH) )
        GAME_2D_LOG_CRITICAL("%s could not be accessed\n\n", SAVE_FILE_PATH);


    // Initialize the SDL subsystems
    if ( SDL_Init(SDL_FLAGS) < 0 || IMG_Init(IMG_FLAGS ) == 0 || TTF_Init() < 0)
    {
        GAME_2D_LOG_CRITICAL("%s\n\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }
}



void Game2D::Run()
{
    Game2D application;

    application.RunInternal();
}



void Game2D::CleanUp()
{
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    CloseSaveFile();

    #ifdef DEBUG
        CloseLogFile();
    #endif
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void Game2D::RunInternal()
{
    float accumulator = 0.0f;
    uint32_t currentTime = SDL_GetTicks();

    m_stateManager.PopState();
    m_stateManager.PushState(MAIN_MENU_STATE);

    while (m_stateManager.GetCurrentState() != QUIT_STATE)
    {
        uint32_t newTime = SDL_GetTicks();
        uint32_t frameTime = (newTime - currentTime);

        if (frameTime > 250)
            frameTime = 250;

        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= TIME_STEP)
        {
            m_stateManager.HandleEvents();
            m_stateManager.Update();

            accumulator -= TIME_STEP;
        }

        m_stateManager.Render();
    }
}



void Game2D::LoadAssets()
{
    Json::Value assets = LoadJson(ASSET_CONFIG_FILE_PATH);


    // Loading Audio
    // =============

    for (const Json::Value& audio : assets["audio"])
    {
        const char* name = audio["name"].asCString();
        const char* path = audio["path"].asCString();

        m_audioManager.LoadAudio(name, path);
    }


    // Loading Fonts
    // =============

    for (const Json::Value& font : assets["fonts"])
    {
        const char* name = font["name"].asCString();
        const char* path = font["path"].asCString();
            
        for (const Json::Value& size : assets["font_sizes"])
            m_fontManager.LoadFont(name, path, size.asUInt());
    }


    // Loading Textures
    // ================

    for (const Json::Value& texture : assets["textures"])
    {
        const char* name = texture["name"].asCString();
        const char* path = texture["path"].asCString();

        m_textureManager.LoadTexture(name, path);
    }
}



void Game2D::CreateMainMenu()
{
    // Create menu background
    // ======================

    SpriteCreateInfo menuInfo;
    menuInfo.texture     = m_textureManager.Get("menu_background");
    menuInfo.animation   = "../../assets/dev/menu_background.json";
    menuInfo.screenCoord = true;

    m_mainMenu.AddBackground(menuInfo);


    // Create play button
    // ==================

    SpriteCreateInfo buttonInfo;
    buttonInfo.texture     = m_textureManager.Get("mother_tick");
    buttonInfo.animation   = "../../assets/dev/mother_tick.json";
    buttonInfo.screenCoord = true;

    Sprite buttonSprite = Sprite(buttonInfo);

    b2Vec2 buttonPos;
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_mainMenu.AddButton("PLAY_BUTTON", buttonSprite, buttonPos);


    // // Create settings button
    // // ======================

    // buttonInfo.texture         = m_textureManager.Get("mother_tick");
    // buttonInfo.animation    = "../../assets/dev/mother_tick.json";
    // buttonInfo.screenCoord = true;

    // buttonSprite = Sprite(buttonInfo);
    // buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    // buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    // m_mainMenu.AddButton("SETTINGS_BUTTON", buttonSprite, buttonPos);


    // // Create quit button
    // // ==================

    // buttonInfo.texture         = m_textureManager.Get("mother_tick");
    // buttonInfo.animation    = "../../assets/dev/mother_tick.json";
    // buttonInfo.screenCoord = true;

    // buttonSprite = Sprite(buttonInfo);
    // buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    // buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    // m_mainMenu.AddButton("QUIT_BUTTON", buttonSprite, buttonPos);
}



void Game2D::CreateSettingsMenu()
{
    // Create menu background
    // ======================

    SpriteCreateInfo menuInfo;
    menuInfo.texture     = m_textureManager.Get("menu_background");
    menuInfo.animation   = "../../assets/dev/menu_background.json";
    menuInfo.screenCoord = true;

    m_settingsMenu.AddBackground(menuInfo);


    // Create back button
    // ==================

    SpriteCreateInfo buttonInfo;
    buttonInfo.texture     = m_textureManager.Get("mother_tick");
    buttonInfo.animation   = "../../assets/dev/mother_tick.json";
    buttonInfo.screenCoord = true;

    Sprite buttonSprite = Sprite(buttonInfo);

    b2Vec2 buttonPos;
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_settingsMenu.AddButton("BACK_BUTTON", buttonSprite, buttonPos);
}



void Game2D::CreatePauseMenu()
{
    // Create menu background
    // ======================

    SpriteCreateInfo menuInfo;
    menuInfo.texture     = m_textureManager.Get("menu_background");
    menuInfo.animation   = "../../assets/dev/menu_background.json";
    menuInfo.screenCoord = true;

    m_pauseMenu.AddBackground(menuInfo);


    // Create resume button
    // ====================

    SpriteCreateInfo buttonInfo;
    buttonInfo.texture     = m_textureManager.Get("mother_tick");
    buttonInfo.animation   = "../../assets/dev/mother_tick.json";
    buttonInfo.screenCoord = true;

    Sprite buttonSprite = Sprite(buttonInfo);

    b2Vec2 buttonPos;
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_pauseMenu.AddButton("RESUME_BUTTON", buttonSprite, buttonPos);


    // Create main menu button
    // =======================

    buttonInfo.texture     = m_textureManager.Get("mother_tick");
    buttonInfo.animation   = "../../assets/dev/mother_tick.json";
    buttonInfo.screenCoord = true;

    buttonSprite = Sprite(buttonInfo);
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_pauseMenu.AddButton("MAIN_MENU_BUTTON", buttonSprite, buttonPos);


    // Create settings button
    // ======================

    buttonInfo.texture     = m_textureManager.Get("mother_tick");
    buttonInfo.animation   = "../../assets/dev/mother_tick.json";
    buttonInfo.screenCoord = true;

    buttonSprite = Sprite(buttonInfo);
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_pauseMenu.AddButton("SETTINGS_BUTTON", buttonSprite, buttonPos);


    // Create quit button
    // ==================

    buttonInfo.texture     = m_textureManager.Get("mother_tick");
    buttonInfo.animation   = "../../assets/dev/mother_tick.json";
    buttonInfo.screenCoord = true;

    buttonSprite = Sprite(buttonInfo);
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_pauseMenu.AddButton("QUIT_BUTTON", buttonSprite, buttonPos);
}



void Game2D::CreateLevelUI()
{
    // Create pause button
    // ===================

    SpriteCreateInfo buttonInfo;
    buttonInfo.texture     = m_textureManager.Get("mother_tick");
    buttonInfo.animation   = "../../assets/dev/mother_tick.json";
    buttonInfo.screenCoord = true;

    Sprite buttonSprite = Sprite(buttonInfo);

    b2Vec2 buttonPos;
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_levelUI.AddButton("PAUSE_BUTTON", buttonSprite, buttonPos);
}



void Game2D::CreateObjects()
{
    Json::Value root = LoadJson(OBJECT_CONFIG_FILE_PATH);


    // Player
    // ======

    m_player.CreateSprite(m_textureManager, root["player"]);
    m_player.CreateHitBox(*m_physicsWorld, root["player"]);

    m_contactListener.LinkToPlayer(m_player);
    m_physicsWorld->SetContactListener(&m_contactListener);


    // Game Objects
    // ============

    for (const Json::Value& object : root["objects"])
    {
        m_objects.emplace_back();
        m_objects.back().CreateSprite(m_textureManager, object);
        m_objects.back().CreateHitBox(*m_physicsWorld, object);
    }


    // Ground Objects
    // ==============

    Json::Value ground = root["ground"];

    for (const Json::Value& position : ground["positions"])
    {
        float xPos = position[0].asFloat();
        float yPos = position[1].asFloat();

        m_objects.emplace_back();
        m_objects.back().CreateSprite(m_textureManager, ground);
        m_objects.back().CreateHitBox(*m_physicsWorld, ground);
        m_objects.back().SetSpawnPoint( b2Vec2(xPos, yPos) );
        m_objects.back().SetPosition( b2Vec2(xPos, yPos) );
    }


    // Enemies
    // =======

    Json::Value enemy = root["obunga"];

    for (const Json::Value& position : enemy["positions"])
    {
        float xPos = position[0].asFloat();
        float yPos = position[1].asFloat();

        m_enemies.emplace_back();
        m_enemies.back().CreateSprite(m_textureManager, enemy);
        m_enemies.back().CreateHitBox(*m_physicsWorld, enemy);
        m_enemies.back().SetSpawnPoint( b2Vec2(xPos, yPos) );
        m_enemies.back().SetPosition( b2Vec2(xPos, yPos) );
    }
}



void Game2D::CreateLayers()
{
    // Create Background Layer
    m_backgroundLayer.reserve(MAX_LAYER_SPRITES);

    SpriteCreateInfo deathStarInfo;
    deathStarInfo.texture     = m_textureManager.Get("death_star");
    deathStarInfo.animation   = "../../assets/dev/death_star.json";
    deathStarInfo.screenCoord = false;
    deathStarInfo.scrollFactor   = 0.01f;

    m_backgroundLayer.emplace_back(deathStarInfo, b2Vec2(7.0f, -1.5f));

    for (auto& [sprite, pos] : m_backgroundLayer)
        sprite.SetAlphaMod(100);


    // Create Foreground Layer
    m_foregroundLayer.reserve(MAX_LAYER_SPRITES);
}
