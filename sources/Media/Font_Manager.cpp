#include <SDL2/SDL_ttf.h>   // SDL_ttf functionality

#include "Media/Text.hpp"   // FontManager class
#include "Utility/Log.hpp"  // GAME_2D_LOG_DEBUG and GAME_2D_LOG_ERROR macro functions


// **************
// IMPLEMENTATION
// **************

void FontManager::LoadFont(std::string_view filepath, uint8_t ptSize)
{
    GAME_2D_LOG_DEBUG("Loading font: %s (point size %d)\n\n", filepath.data(), ptSize);


    TTF_Font* font = TTF_OpenFont(filepath.data(), ptSize);

    if (!font)
    {
        GAME_2D_LOG_ERROR("%s\n\n", TTF_GetError());
        return;
    }


    auto key = std::make_pair(filepath.data(), ptSize);

    m_fonts.emplace(key, font);
}



_TTF_Font* FontManager::Get(std::string_view filepath, uint8_t ptSize)
{
    auto key = std::make_pair(filepath.data(), ptSize);

    return m_fonts[key];
}



FontManager::~FontManager()
{
    for (auto& [key, font] : m_fonts)
    {
        [[maybe_unused]] const auto& [filepath, ptSize] = key;

        GAME_2D_LOG_DEBUG("Destroying font: %s (point size %d)\n\n", filepath.data(), ptSize);

        TTF_CloseFont(font);
    }
}
