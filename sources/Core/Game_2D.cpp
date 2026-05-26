#include <box2d/box2d.h>            // Box2D functionality
#include <SDL2/SDL.h>               // SDL functionality
#include <SDL2/SDL_image.h>         // IMG_GetError and IMG_Quit functions
#include <SDL2/SDL_ttf.h>           // TTF_Init and TTF_Quit functions, TTF_Font struct

#include <array>                    // std::array
#include <cmath>                    // M_PI constant

#ifndef __GNUC__
    #include <filesystem>           // std::filesystem
#endif

#include <iostream>                 // std::cerr
#include <string>                   // std::string

#include "Core/Game_2D.hpp"         // Game2D class
#include "Utility/Log.hpp"          // InitLog, CloseLogFile and Log macro functions
#include "Utility/Math.hpp"         // ConvertToMeters function
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

static constexpr const char* LOG_FILE_PATH  = "../../sources/log.txt";
static constexpr const char* SAVE_FILE_PATH = "../../sources/save.json";


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
static constexpr std::array FONT_SIZES        = { 12, 18, 24, 36, 48, 60 };
static const     b2Vec2     G_ACCELERATION    = b2Vec2(0.0f, -9.81f);



// **************
// IMPLEMENTATION
// **************

Game2D::Game2D():
    m_window         ("2D_PLATFORMER", WINDOW_WIDTH, WINDOW_HEIGHT),
    m_renderer       (m_window),
    m_textureManager (m_renderer),
    m_physicsWorld   (std::make_unique<b2World>(G_ACCELERATION))
{
    LoadAssets();

    CreateMainMenu();
    CreateSettingsMenu();
    CreatePauseMenu();
    // CreateLevelUI();

    CreatePlayer();
    CreateObjects();
    CreateEnemies();
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
        if (!InitLog(LOG_FILE_PATH, LOG_PRIORITY_DEBUG))
            std::cerr << "\aERROR: " << LOG_FILE_PATH << " could not be accessed\n";
    #endif


    // Initialize the save system
    if (!OpenSaveFile(SAVE_FILE_PATH))
        GAME_2D_LOG_CRITICAL("%s could not be accessed\n\n", SAVE_FILE_PATH);


    // Initialize the SDL subsystems
    if (SDL_Init(SDL_FLAGS) < 0 || IMG_Init(IMG_FLAGS) == 0 || TTF_Init() < 0)
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
    // Loading Audio
    m_audioManager.LoadAudio("../../assets/audio/8_bit_music.wav"              );
    m_audioManager.LoadAudio("../../assets/audio/God-of-War-Ragnarok-Theme.wav");
    m_audioManager.LoadAudio("../../assets/audio/God-of-War-Theme.wav"         );
    m_audioManager.LoadAudio("../../assets/audio/hit_ground.wav"               );
    m_audioManager.LoadAudio("../../assets/audio/videogame-death-sound.wav"    );


    // Loading Fonts
    for (uint8_t ptSize : FONT_SIZES)
    {
        m_fontManager.LoadFont("../../assets/fonts/cocogoose.ttf", ptSize);
        m_fontManager.LoadFont("../../assets/fonts/runescape.ttf", ptSize);
    }


    // Loading Textures
    m_textureManager.LoadTexture("../../assets/gfx/mother_tick-Sheet.png");
    m_textureManager.LoadTexture("../../assets/gfx/wood_block.png"       );
    m_textureManager.LoadTexture("../../assets/gfx/soccer_ball.png"      );
    m_textureManager.LoadTexture("../../assets/gfx/grass_block.png"      );
    m_textureManager.LoadTexture("../../assets/gfx/obunga.jpg"           );
    m_textureManager.LoadTexture("../../assets/gfx/ground.png"           );
    m_textureManager.LoadTexture("../../assets/gfx/death_star.png"       );
    m_textureManager.LoadTexture("../../assets/gfx/menu_background.png"  );
}



void Game2D::CreateMainMenu()
{
    // Create menu background
    // ======================

    SpriteCreateInfo menuInfo;
    menuInfo.texture        = m_textureManager.Get("../../assets/gfx/menu_background.png");
    menuInfo.dataFilePath   = "../../assets/dev/menu_background.json";
    menuInfo.useScreenCoord = true;

    m_mainMenu.AddBackground(menuInfo);


    // Create play button
    // ==================

    SpriteCreateInfo buttonInfo;
    buttonInfo.texture        = m_textureManager.Get("../../assets/gfx/mother_tick-Sheet.png");
    buttonInfo.dataFilePath   = "../../assets/dev/mother_tick.json";
    buttonInfo.useScreenCoord = true;

    Sprite buttonSprite = Sprite(buttonInfo);

    b2Vec2 buttonPos;
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_mainMenu.AddButton("PLAY_BUTTON", buttonSprite, buttonPos);


    // // Create settings button
    // // ======================

    // buttonInfo.texture         = m_textureManager.Get("../../assets/gfx/mother_tick-Sheet.png");
    // buttonInfo.dataFilePath    = "../../assets/dev/mother_tick.json";
    // buttonInfo.useScreenCoord = true;

    // buttonSprite = Sprite(buttonInfo);
    // buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    // buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    // m_mainMenu.AddButton("SETTINGS_BUTTON", buttonSprite, buttonPos);


    // // Create quit button
    // // ==================

    // buttonInfo.texture         = m_textureManager.Get("../../assets/gfx/mother_tick-Sheet.png");
    // buttonInfo.dataFilePath    = "../../assets/dev/mother_tick.json";
    // buttonInfo.useScreenCoord = true;

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
    menuInfo.texture        = m_textureManager.Get("../../assets/gfx/menu_background.png");
    menuInfo.dataFilePath   = "../../assets/dev/menu_background.json";
    menuInfo.useScreenCoord = true;

    m_settingsMenu.AddBackground(menuInfo);


    // Create back button
    // ==================

    SpriteCreateInfo buttonInfo;
    buttonInfo.texture        = m_textureManager.Get("../../assets/gfx/mother_tick-Sheet.png");
    buttonInfo.dataFilePath   = "../../assets/dev/mother_tick.json";
    buttonInfo.useScreenCoord = true;

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
    menuInfo.texture        = m_textureManager.Get("../../assets/gfx/menu_background.png");
    menuInfo.dataFilePath   = "../../assets/dev/menu_background.json";
    menuInfo.useScreenCoord = true;

    m_pauseMenu.AddBackground(menuInfo);


    // Create resume button
    // ====================

    SpriteCreateInfo buttonInfo;
    buttonInfo.texture        = m_textureManager.Get("../../assets/gfx/mother_tick-Sheet.png");
    buttonInfo.dataFilePath   = "../../assets/dev/mother_tick.json";
    buttonInfo.useScreenCoord = true;

    Sprite buttonSprite = Sprite(buttonInfo);

    b2Vec2 buttonPos;
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_pauseMenu.AddButton("RESUME_BUTTON", buttonSprite, buttonPos);


    // Create main menu button
    // =======================

    buttonInfo.texture        = m_textureManager.Get("../../assets/gfx/mother_tick-Sheet.png");
    buttonInfo.dataFilePath   = "../../assets/dev/mother_tick.json";
    buttonInfo.useScreenCoord = true;

    buttonSprite = Sprite(buttonInfo);
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_pauseMenu.AddButton("MAIN_MENU_BUTTON", buttonSprite, buttonPos);


    // Create settings button
    // ======================

    buttonInfo.texture        = m_textureManager.Get("../../assets/gfx/mother_tick-Sheet.png");
    buttonInfo.dataFilePath   = "../../assets/dev/mother_tick.json";
    buttonInfo.useScreenCoord = true;

    buttonSprite = Sprite(buttonInfo);
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_pauseMenu.AddButton("SETTINGS_BUTTON", buttonSprite, buttonPos);


    // Create quit button
    // ==================

    buttonInfo.texture        = m_textureManager.Get("../../assets/gfx/mother_tick-Sheet.png");
    buttonInfo.dataFilePath   = "../../assets/dev/mother_tick.json";
    buttonInfo.useScreenCoord = true;

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
    buttonInfo.texture        = m_textureManager.Get("../../assets/gfx/mother_tick-Sheet.png");
    buttonInfo.dataFilePath   = "../../assets/dev/mother_tick.json";
    buttonInfo.useScreenCoord = true;

    Sprite buttonSprite = Sprite(buttonInfo);

    b2Vec2 buttonPos;
    buttonPos.x = (m_window.GetWidth() / 2.0f) - (buttonSprite.GetFrameWidth() / 2.0f);
    buttonPos.y = (m_window.GetHeight() / 2.0f) - (buttonSprite.GetFrameHeight() / 2.0f);

    m_levelUI.AddButton("PAUSE_BUTTON", buttonSprite, buttonPos);
}



void Game2D::CreatePlayer()
{
    // Create Player
    // =============

    // Create player sprite
    SpriteCreateInfo playerInfo;
    playerInfo.texture        = m_textureManager.Get("../../assets/gfx/mother_tick-Sheet.png");
    playerInfo.dataFilePath   = "../../assets/dev/mother_tick.json";
    playerInfo.useScreenCoord = false;

    m_player.CreateSprite(playerInfo);


    // Create player physics body
    b2BodyDef playerBodyDef;
    playerBodyDef.type          = b2_dynamicBody;
    playerBodyDef.position      = b2Vec2(0.7f, 16.0f);
    playerBodyDef.fixedRotation = true;

    m_player.CreateBody(*m_physicsWorld, playerBodyDef);


    // Create player shape
    b2PolygonShape playerBox;
    float width = ConvertToMeters( m_player.GetSprite().GetFrameWidth() );
    float height = ConvertToMeters( m_player.GetSprite().GetFrameHeight() );

    playerBox.SetAsBox(width / 2.0f, height / 2.0f);


    // Create player fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape    = &playerBox;
    fixtureDef.density  = 5.0f;
    fixtureDef.friction = 1000.0f;

    m_player.CreateMainFixture(fixtureDef);


    // Create player foot sensor
    b2EdgeShape sensorShape;
    b2Vec2 vertex1 = b2Vec2(-width / 2.0f, -height / 2.0f);
    b2Vec2 vertex2 = b2Vec2( width / 2.0f, -height / 2.0f);

    sensorShape.SetTwoSided(vertex1, vertex2);

    b2FixtureDef sensorDef;
    sensorDef.shape    = &sensorShape;
    sensorDef.density  = 1.0f;
    sensorDef.isSensor = true;
    sensorDef.friction = 1000.0f;

    m_player.CreateGroundSensor(sensorDef);


    // Set up player-ground collision system
    m_contactListener.LinkToPlayer(m_player);
    m_physicsWorld->SetContactListener(&m_contactListener);
}



void Game2D::CreateObjects()
{
    m_objects.reserve(MAX_OBJECTS);


    // Kinematic Wood Block
    // ====================

    m_objects.emplace_back();


    // Create wood block sprite
    SpriteCreateInfo woodInfo;
    woodInfo.texture        = m_textureManager.Get("../../assets/gfx/wood_block.png");
    woodInfo.dataFilePath   = "../../assets/dev/wood_block.json";
    woodInfo.useScreenCoord = false;

    m_objects[0].CreateSprite(woodInfo);


    // Create wood block physics body
    b2BodyDef woodBodyDef;
    woodBodyDef.type          = b2_kinematicBody;
    woodBodyDef.angle         = static_cast<float>(M_PI) / 2.0f;
    woodBodyDef.position      = b2Vec2(-20.0f, 3.5f);
    woodBodyDef.fixedRotation = true;

    m_objects[0].CreateBody(*m_physicsWorld, woodBodyDef);


    // Create wood block shape
    b2PolygonShape woodBox;
    float woodWidth = ConvertToMeters( m_objects[0].GetSprite().GetFrameWidth() );
    float woodHeight = ConvertToMeters( m_objects[0].GetSprite().GetFrameHeight() );

    woodBox.SetAsBox(woodWidth / 2.0f, woodHeight / 2.0f);


    // Create wood block fixture
    b2FixtureDef woodFixtureDef;
    woodFixtureDef.shape = &woodBox;

    m_objects[0].CreateMainFixture(woodFixtureDef);



    // Dynamic Soccer Ball
    // ===================

    m_objects.emplace_back();


    // Create soccer ball sprite
    SpriteCreateInfo ballInfo;
    ballInfo.texture        = m_textureManager.Get("../../assets/gfx/soccer_ball.png");
    ballInfo.dataFilePath   = "../../assets/dev/soccer_ball.json";
    ballInfo.useScreenCoord = false;

    m_objects[1].CreateSprite(ballInfo);



    // Create soccer ball physics body
    b2BodyDef ballBodyDef;
    ballBodyDef.type          = b2_dynamicBody;
    ballBodyDef.position      = b2Vec2(0.8f, 20.0f);
    ballBodyDef.fixedRotation = false;

    m_objects[1].CreateBody(*m_physicsWorld, ballBodyDef);


    // Create soccer ball shape
    b2CircleShape ballShape;

    float ballDiameter = ConvertToMeters( m_objects[1].GetSprite().GetFrameWidth() );
    ballShape.m_radius = ballDiameter / 2.0f;


    // Create soccer ball fixture
    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape       = &ballShape;
    ballFixtureDef.density     = 1.0f;
    ballFixtureDef.friction    = 0.7f;
    ballFixtureDef.restitution = 0.7f;

    m_objects[1].CreateMainFixture(ballFixtureDef);



    // Dynamic Grass Block
    // ===================

    m_objects.emplace_back();


    // Create grass block sprite
    SpriteCreateInfo grassInfo;
    grassInfo.texture        = m_textureManager.Get("../../assets/gfx/grass_block.png");
    grassInfo.dataFilePath   = "../../assets/dev/grass_block.json";
    grassInfo.useScreenCoord = false;

    m_objects[2].CreateSprite(grassInfo);


    // Create grass block physics body
    b2BodyDef grassBodyDef;
    grassBodyDef.type          = b2_dynamicBody;
    grassBodyDef.position      = b2Vec2(0.7f, 25.0f);
    grassBodyDef.fixedRotation = false;

    m_objects[2].CreateBody(*m_physicsWorld, grassBodyDef);


    // Create grass block shape
    b2PolygonShape grassShape;
    float grassWidth = ConvertToMeters( m_objects[2].GetSprite().GetFrameWidth() );
    float grassHeight = ConvertToMeters( m_objects[2].GetSprite().GetFrameHeight() );

    grassShape.SetAsBox(grassWidth / 2.0f, grassHeight / 2.0f);


    // Create grass block fixture
    b2FixtureDef grassFixtureDef;
    grassFixtureDef.shape       = &grassShape;
    grassFixtureDef.density     = 2.0f;
    grassFixtureDef.friction    = 1000.0f;
    grassFixtureDef.restitution = 0.05f;

    m_objects[2].CreateMainFixture(grassFixtureDef);



    // Create Static Ground Objects
    // ============================

    b2Vec2 pos = b2Vec2(0.0f, 0.0f);

    for (uint64_t i = 3; i < MAX_OBJECTS; ++i)
    {
        m_objects.emplace_back();


        // Create ground sprites
        SpriteCreateInfo groundInfo;
        groundInfo.texture        = m_textureManager.Get("../../assets/gfx/ground.png");
        groundInfo.dataFilePath   = "../../assets/dev/ground.json";
        groundInfo.useScreenCoord = false;

        m_objects[i].CreateSprite(groundInfo);


        // Create ground physics bodies
        b2BodyDef groundBodyDef;
        groundBodyDef.type          = b2_staticBody;
        groundBodyDef.position      = pos;
        groundBodyDef.fixedRotation = true;

        m_objects[i].CreateBody(*m_physicsWorld, groundBodyDef);


        // Create ground shapes
        b2PolygonShape groundShape;
        float groundWidth = ConvertToMeters( m_objects[i].GetSprite().GetFrameWidth() );
        float groundHeight = ConvertToMeters( m_objects[i].GetSprite().GetFrameHeight() );

        groundShape.SetAsBox(groundWidth / 2.0f, groundHeight / 2.0f);


        // Create ground fixtures
        b2FixtureDef groundFixtureDef;
        groundFixtureDef.shape = &groundShape;

        m_objects[i].CreateMainFixture(groundFixtureDef);


        pos.x += groundWidth;
        pos.y -= 0.5f;
    }
}



void Game2D::CreateEnemies()
{
    m_enemies.reserve(MAX_ENEMIES);


    b2Vec2 pos = b2Vec2(100.0f, -7.0f);

    for (uint64_t i = 0; i < MAX_ENEMIES; ++i)
    {
        m_enemies.emplace_back();


        // Create enemy sprites
        SpriteCreateInfo enemyInfo;
        enemyInfo.texture        = m_textureManager.Get("../../assets/gfx/obunga.jpg");
        enemyInfo.dataFilePath   = "../../assets/dev/obunga.json";
        enemyInfo.useScreenCoord = false;

        m_enemies[i].CreateSprite(enemyInfo);


        // Create enemy physics bodies
        b2BodyDef enemyBodyDef;
        enemyBodyDef.type          = b2_dynamicBody;
        enemyBodyDef.position      = pos;
        enemyBodyDef.fixedRotation = false;

        m_enemies[i].CreateBody(*m_physicsWorld, enemyBodyDef);


        // Create enemy shapes
        b2PolygonShape enemyShape;
        float width = ConvertToMeters( m_enemies[i].GetSprite().GetFrameWidth() );
        float height = ConvertToMeters( m_enemies[i].GetSprite().GetFrameHeight() );

        enemyShape.SetAsBox(width / 2.0f, height / 2.0f);


        // Create enemy fixtures
        b2FixtureDef enemyFixtureDef;
        enemyFixtureDef.shape    = &enemyShape;
        enemyFixtureDef.density  = 0.7f;
        enemyFixtureDef.friction = 1.0f;

        m_enemies[i].CreateMainFixture(enemyFixtureDef);


        pos.x += 0.25f;
        pos.y += width;
    }
}



void Game2D::CreateLayers()
{
    // Create Background Layer
    m_backgroundLayer.reserve(MAX_LAYER_SPRITES);

    SpriteCreateInfo deathStarInfo;
    deathStarInfo.texture        = m_textureManager.Get("../../assets/gfx/death_star.png");
    deathStarInfo.dataFilePath   = "../../assets/dev/death_star.json";
    deathStarInfo.useScreenCoord = false;
    deathStarInfo.scrollFactor   = 0.01f;

    m_backgroundLayer.emplace_back(deathStarInfo, b2Vec2(7.0f, -1.5f));

    for (auto& [sprite, pos] : m_backgroundLayer)
        sprite.SetAlphaMod(100);


    // Create Foreground Layer
    m_foregroundLayer.reserve(MAX_LAYER_SPRITES);
}
