#include <json/value.h>         // Json::Value class
#include <SDL3/SDL_render.h>    // SDL_SetTextureAlphaMod function

#include "Entity/Sprite.hpp"    // Sprite class
#include "Utility/Assert.hpp"   // GAME_2D_ASSERT macro function
#include "Utility/Log.hpp"      // GAME_2D_LOG_VERBOSE macro function
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
    m_texture      (nullptr),
    m_screenCoord  (true),
    m_scrollFactor (1.0f)
{
    m_animations.reserve(MAX_SPRITE_ANIMATIONS);
}



Sprite::Sprite(const SpriteCreateInfo& info):
    m_texture      (info.texture),
    m_screenCoord  (info.screenCoord),
    m_scrollFactor (info.scrollFactor)
{
    GAME_2D_ASSERT( !info.animation.empty() );

    LoadAnimations(info.animation);
}



void Sprite::operator=(const SpriteCreateInfo& info)
{
    m_texture      = info.texture;
    m_screenCoord  = info.screenCoord;
    m_scrollFactor = info.scrollFactor;

    GAME_2D_ASSERT( !info.animation.empty() );

    LoadAnimations(info.animation);
}



void Sprite::LoadAnimations(const std::string& filepath)
{
    Json::Value root = LoadJson(filepath);

    std::vector<Frame> frames;
    frames.reserve(MAX_ANIMATION_FRAMES);


    // To store the data for every individual sprite frame
    for (const Json::Value& frame : root["frames"])
    {
        SDL_FRect srcrect;
        srcrect.x = frame["frame"]["x"].asFloat();
        srcrect.y = frame["frame"]["y"].asFloat();
        srcrect.w = frame["frame"]["w"].asFloat();
        srcrect.h = frame["frame"]["h"].asFloat();
 
        uint8_t duration = frame["duration"].asUInt();

        frames.emplace_back(srcrect, duration);
    }


    // To create animation sets from the vector of frame structures
    for (const Json::Value& tag : root["meta"]["frameTags"])
    {
        Animation cycle;

        std::string name  = tag["name"].asString();
        uint8_t     begin = tag["from"].asUInt();
        uint8_t     end   = tag["to"].asUInt();

        // Create an animation set from a subset of frames
        for (uint8_t k = begin; k <= end; ++k)
            cycle.AddFrame(frames[k]);

        m_animations.emplace(name, cycle);
    }


    // All sprite meta data has a default animation frame
    m_currentFrame = m_animations["default"].GetCurrentFrame();
}



const SDL_FRect& Sprite::GetSourceRect() const
{
    return m_currentFrame.srcrect;
}



const SDL_Texture* Sprite::GetTexture() const
{
    return m_texture;
}



float Sprite::GetFrameWidth() const
{
    return m_currentFrame.srcrect.w;
}



float Sprite::GetFrameHeight() const
{
    return m_currentFrame.srcrect.h;
}



bool Sprite::UseScreenCoord() const
{
    return m_screenCoord;
}



float Sprite::GetScrollFactor() const
{
    return m_scrollFactor;
}



void Sprite::SetAlphaMod(uint8_t alpha)
{
    SDL_SetTextureAlphaMod( const_cast<SDL_Texture*>(m_texture), alpha );
}



void Sprite::ResetAnimation(const std::string& name)
{
    if ( !m_animations.contains(name) )
        return;

    m_animations[name].Reset();
}



void Sprite::PlayAnimation(const std::string& name)
{
    if ( !m_animations.contains(name) )
    {
        GAME_2D_LOG_VERBOSE("Could not display \'%s\' animation.\n", name.data());
        return;
    }

    m_animations[name].Play();

    m_currentFrame = m_animations[name].GetCurrentFrame();
}
