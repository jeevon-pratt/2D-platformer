#include <SDL3/SDL_render.h>            // SDL_Renderer struct, SDL_DestroyTexture function
#include <SDL3_image/SDL_image.h>       // IMG_LoadTexture functions

#include "Media/Texture_Manager.hpp"    // TextureManager class
#include "Media/Renderer.hpp"           // Renderer class
#include "Utility/Log.hpp"              // GAME_2D_LOG_DEBUG and GAME_2D_LOG_ERROR macro functions


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr uint16_t MAX_TEXTURES = 500;



// **************
// IMPLEMENTATION
// **************

TextureManager::TextureManager(const Renderer& renderer):
    m_rendererContext ( renderer.GetContext() )
{
    m_textures.reserve(MAX_TEXTURES);
}



void TextureManager::LoadTexture(const std::string& name, const std::string& filepath)
{
    GAME_2D_LOG_DEBUG("Loading texture: %s\n\n", filepath.data());


    SDL_Renderer* context = const_cast<SDL_Renderer*>(m_rendererContext);
    SDL_Texture*  texture = IMG_LoadTexture(context, filepath.data());

    if (!texture)
    {
        GAME_2D_LOG_ERROR("Could not load texture, %s\n\n", SDL_GetError());
        return;
    }

    m_textures.emplace(name, texture);
}



const SDL_Texture* TextureManager::Get(const std::string& name) const
{
    if ( !m_textures.contains(name) )
    {
        GAME_2D_LOG_ERROR("Could not find texture %s\n\n", name.data());
        return nullptr;
    }

    return m_textures.at(name);
}



TextureManager::~TextureManager()
{
    for (auto& [name, texture] : m_textures)
    {
        GAME_2D_LOG_DEBUG("Destroying texture: %s\n\n", name.data());
        SDL_DestroyTexture( const_cast<SDL_Texture*>(texture) );
    }
}