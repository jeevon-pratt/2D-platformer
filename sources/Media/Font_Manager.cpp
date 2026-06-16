#include <string>           // std::string, std::to_string

#include "Media/Text.hpp"   // FontManager class
#include "Utility/Log.hpp"  // GAME_2D_LOG_DEBUG and GAME_2D_LOG_ERROR macro functions


// **************
// IMPLEMENTATION
// **************

void FontManager::LoadFont(const std::string& name, const std::string& filepath, uint8_t size)
{
    GAME_2D_LOG_DEBUG("Loading font: %s (point size: %d)\n\n", filepath.data(), size);

     // The font size is incorporated into the hash table key
    std::string key  = std::string(name) + "_" + std::to_string(size);
    TTF_Font*   font = TTF_OpenFont(filepath.data(), size);

    if (!font)
    {
        GAME_2D_LOG_ERROR("%s\n\n", TTF_GetError());
        return;
    }

    m_fonts.emplace(key, font);
}



const TTF_Font* FontManager::Get(const std::string& name) const
{
    if ( !m_fonts.contains(name) )
    {
        GAME_2D_LOG_ERROR("Could not find font %s\n\n", name.data());
        return nullptr;
    }

    return m_fonts.at(name);
}




FontManager::~FontManager()
{
    for (auto& [name, font] : m_fonts)
    {
        GAME_2D_LOG_DEBUG("Destroying font: %s\n\n", name.data());
        TTF_CloseFont( const_cast<TTF_Font*>(font) );
    }
}
