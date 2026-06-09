#include <json/value.h>         // Json::Value class
#include <SDL2/SDL_render.h>    // SDL_SetTextureAlphaMod function

#include <fstream>              // std::ifstream

#include "Media/Sprite.hpp"     // Sprite class
#include "Utility/Log.hpp"      // GAME_2D_LOG_ERROR macro function
#include "Utility/Memory.hpp"   // LoadJson function


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr uint8_t MAX_SPRITE_ANIMATIONS = 10;
static constexpr uint8_t MAX_ANIMATION_FRAMES  = 50;



// **************
// IMPLEMENTATION
// **************

Sprite::Sprite():
    m_texture        (nullptr),
    m_currentFrame   { 0, 0, 0, 0 },
    m_useScreenCoord (true),
    m_scrollFactor   (1.0f)
{
    m_animations.reserve(MAX_SPRITE_ANIMATIONS);
}



Sprite::Sprite(const SpriteCreateInfo& info):
    m_texture        (info.texture),
    m_currentFrame   { 0, 0, 0, 0 },
    m_useScreenCoord (info.useScreenCoord),
    m_scrollFactor   (info.scrollFactor)
{
    if (!info.dataFilePath.empty())
        LoadAnimations(info.dataFilePath);
}



void Sprite::LoadAnimations(std::string_view filepath)
{
    // Sprite data
    Json::Value root   = LoadJson(filepath);
    Json::Value frames = root["frames"];
    Json::Value meta   = root["meta"];
    Json::Value tags   = meta["frameTags"];

    // Every animation frame for the sprite
    std::vector<Frame> totalFrames;
    totalFrames.reserve(MAX_ANIMATION_FRAMES);


    // To store the data for every individual sprite frame
    for (const Json::Value& currentFrame : frames)
    {
        Json::Value rect = currentFrame["frame"];

        SDL_Rect srcrect;
        srcrect.x = rect["x"].asInt();
        srcrect.y = rect["y"].asInt();
        srcrect.w = rect["w"].asInt();
        srcrect.h = rect["h"].asInt();

        uint8_t duration = currentFrame["duration"].asUInt();

        totalFrames.emplace_back( Frame{ srcrect, duration } );
    }


    // To create animation sets from the vector of frame structures
    for (const Json::Value& currentTag : tags)
    {
        Animation cycle;
        std::string cycleName = currentTag["name"].asCString();

        uint8_t begin = currentTag["from"].asUInt();
        uint8_t end   = currentTag["to"].asUInt();

        // Create an animation set from a subset of frames
        for (uint8_t k = begin; k <= end; ++k)
            cycle.AddFrame(totalFrames[k]);

        m_animations.emplace(cycleName, cycle);
    }


    // All sprite meta data has a default animation frame
    m_currentFrame = m_animations["default"].GetCurrentFrame();
}



const SDL_Rect& Sprite::GetSourceRect() const
{
    return m_currentFrame.srcrect;
}



const SDL_Texture* Sprite::GetTexture() const
{
    return m_texture;
}



uint16_t Sprite::GetFrameWidth() const
{
    return m_currentFrame.srcrect.w;
}



uint16_t Sprite::GetFrameHeight() const
{
    return m_currentFrame.srcrect.h;
}



bool Sprite::UseScreenCoord() const
{
    return m_useScreenCoord;
}



float Sprite::GetScrollFactor() const
{
    return m_scrollFactor;
}



void Sprite::SetAlphaMod(uint8_t alpha)
{
    SDL_SetTextureAlphaMod( const_cast<SDL_Texture*>(m_texture), alpha );
}



void Sprite::PlayAnimation(std::string_view name)
{
    if ( m_animations.count(name.data()) == 0 )
    {
        GAME_2D_LOG_ERROR("Could not display \'%s\' animation.\n\n", name.data());
        return;
    }


    Animation& cycle = m_animations[ name.data() ];

    cycle.StepTime();

    if (cycle.FrameTimeExceeded())
    {
        cycle.NextFrame();

        m_currentFrame = cycle.GetCurrentFrame();
    }
}
