#include <SDL2/SDL_render.h>            // SDL_Renderer struct, SDL_DestroyTexture function
#include <SDL2/SDL_image.h>             // IMG_GetError and IMG_LoadTexture functions

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



void TextureManager::LoadTexture(std::string_view name, std::string_view filepath)
{
    GAME_2D_LOG_DEBUG("Loading texture: %s\n\n", filepath.data());


    SDL_Texture* texture = IMG_LoadTexture( const_cast<SDL_Renderer*>(m_rendererContext), filepath.data() );

    if (!texture)
    {
        GAME_2D_LOG_ERROR("%s\n\n", IMG_GetError());
        return;
    }

    m_textures.emplace(name.data(), texture);
}



const SDL_Texture* TextureManager::Get(std::string_view name) const
{
    if ( m_textures.count(name.data()) == 0 )
    {
        GAME_2D_LOG_ERROR("Could not find texture %s\n\n", name.data());
        return nullptr;
    }

    return m_textures.at( name.data() );
}



TextureManager::~TextureManager()
{
    for (auto& [name, texture] : m_textures)
    {
        GAME_2D_LOG_DEBUG("Destroying texture: %s\n\n", name.data());
        SDL_DestroyTexture( const_cast<SDL_Texture*>(texture) );
    }
}