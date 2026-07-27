#include <json/value.h>             // Json::Value class
#include <SDL3/SDL_render.h>        // SDL_Texture struct

#include "Media/Asset.hpp"          // AssetManager class
#include "Media/Audio.hpp"          // Audio class
#include "Media/Renderer.hpp"       // Renderer class


// **************
// IMPLEMENTATION
// **************

AssetManager::AssetManager(const Renderer& renderer):
    m_textureManager (renderer)
{
}



void AssetManager::LoadAudio(const Json::Value& asset)
{
    m_audioManager.LoadAudio(asset);
}



void AssetManager::LoadFont(const Json::Value& asset, uint8_t size)
{
    m_fontManager.LoadFont(asset, size);
}



void AssetManager::LoadTexture(const Json::Value& asset)
{
    m_textureManager.LoadTexture(asset);
}



Audio AssetManager::GetAudio(const std::string& name) const
{
    return m_audioManager.GetAudio(name);
}



const TTF_Font* AssetManager::GetFont(const std::string& name) const
{
    return m_fontManager.GetFont(name);
}



const SDL_Texture* AssetManager::GetTexture(const std::string& name) const
{
    return m_textureManager.GetTexture(name);
}
