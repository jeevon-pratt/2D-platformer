#include <json/value.h>                 // Json::Value class
#include <SDL3/SDL_render.h>            // SDL_Renderer struct, SDL_DestroyTexture function
#include <SDL3_image/SDL_image.h>       // IMG_LoadTexture functions

#include "Media/Asset.hpp"              // TextureManager class
#include "Media/Renderer.hpp"           // Renderer class
#include "Utility/Assert.hpp"           // GAME_2D_ASSERT macro function
#include "Utility/Log.hpp"              // Log macro functions


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr uint16_t MAX_TEXTURES = 100;



// **************
// IMPLEMENTATION
// **************

TextureManager::TextureManager(const Renderer& renderer):
    m_rendererContext (renderer.GetContext())
{
    m_registry.reserve(MAX_TEXTURES);
}



void TextureManager::LoadTexture(const Json::Value& asset)
{
    const char* name = asset["name"].asCString();
    const char* path = asset["path"].asCString();

    GAME_2D_LOG_DEBUG("Loading texture: %s\n", path);


    SDL_Renderer* renderer = const_cast<SDL_Renderer*>(m_rendererContext);
    SDL_Texture*  texture  = IMG_LoadTexture(renderer, path);

    if (!texture)
    {
        GAME_2D_LOG_ERROR("%s\n", SDL_GetError());
        return;
    }

    m_registry.emplace(name, texture);

    GAME_2D_ASSERT(m_registry.size() <= MAX_TEXTURES);
}



const SDL_Texture* TextureManager::GetTexture(const std::string& name) const
{
    if (!m_registry.contains(name))
    {
        GAME_2D_LOG_ERROR("Could not find texture %s\n", name.data());
        return nullptr;
    }

    return m_registry.at(name);
}



TextureManager::~TextureManager()
{
    for (auto& [name, texture] : m_registry)
    {
        GAME_2D_LOG_DEBUG("Destroying texture: %s\n", name.data());
        SDL_DestroyTexture(texture);
    }
}