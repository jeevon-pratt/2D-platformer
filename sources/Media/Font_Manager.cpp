#include <json/value.h>     // Json::Value class

#include <format>           // std::format
#include <string>           // std::string

#include "Media/Asset.hpp"  // FontManager class
#include "Utility/Log.hpp"  // GAME_2D_LOG_DEBUG and GAME_2D_LOG_ERROR macro functions


// **************
// IMPLEMENTATION
// **************

void FontManager::LoadFont(const Json::Value& asset, uint8_t size)
{
    const char* name = asset["name"].asCString();
    const char* path = asset["path"].asCString();

    GAME_2D_LOG_DEBUG("Loading font: %s (point size: %d)\n", path, size);

     // The font size is incorporated into the hash table key
    std::string key  = std::format("{}_{}", name, size);
    TTF_Font*   data = TTF_OpenFont(path, size);

    if (!data)
    {
        GAME_2D_LOG_ERROR("%s\n", SDL_GetError());
        return;
    }

    m_registry.emplace(key, data);

}



const TTF_Font* FontManager::GetFont(const std::string& name) const
{
    if (!m_registry.contains(name))
    {
        GAME_2D_LOG_ERROR("Could not find font %s\n\n", name.c_str());
        return nullptr;
    }

    return m_registry.at(name);
}




FontManager::~FontManager()
{
    for (auto& [name, font] : m_registry)
    {
        GAME_2D_LOG_DEBUG("Destroying font: %s\n", name.c_str());
        TTF_CloseFont(font);
    }
}
