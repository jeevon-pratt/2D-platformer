#include <json/value.h>         // Json::Value class
#include <SDL2/SDL_render.h>    // SDL_SetTextureAlphaMod function

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
    if (!info.animation.empty())
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
        SDL_Rect srcrect;
        srcrect.x = frame["frame"]["x"].asInt();
        srcrect.y = frame["frame"]["y"].asInt();
        srcrect.w = frame["frame"]["w"].asInt();
        srcrect.h = frame["frame"]["h"].asInt();
 
        uint8_t duration = frame["duration"].asUInt();

        frames.emplace_back( Frame{ srcrect, duration } );
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



void Sprite::PlayAnimation(const std::string& name)
{
    if ( !m_animations.contains(name) )
    {
        GAME_2D_LOG_ERROR("Could not display \'%s\' animation.\n\n", name.data());
        return;
    }


    Animation& cycle = m_animations[name];

    cycle.StepTime();

    if (cycle.FrameTimeExceeded())
    {
        cycle.NextFrame();

        m_currentFrame = cycle.GetCurrentFrame();
    }
}
