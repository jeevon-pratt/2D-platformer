#pragma once

#include <SDL3_ttf/SDL_ttf.h>           // TTF_Font struct

#include <string>                       // std::string
#include <unordered_map>                // std::unordered_map

class  Audio;
class  Renderer;
struct MIX_Audio;
struct MIX_Mixer;
struct SDL_Renderer;
struct SDL_Texture;

namespace Json
{
    class Value;
}


// *******************
// AUDIO MANAGER CLASS
// *******************

/**
 *  Class for managing the creation and destruction of Audio objects
 */
class AudioManager final
{
public:
    // IMPLEMENTATION
    // ==============

    // Default constructor
    AudioManager();

    // Loads audio data with the associated JSON asset
    void LoadAudio(const Json::Value& asset);

    // Returns the Audio object with the specified name
    Audio GetAudio(const std::string& name) const;

    // Frees the audio data loaded by the manager
    ~AudioManager();

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and assignment operator have been disabled to
    //       prevent the copying of audio assets.
    AudioManager(const AudioManager& manager) = delete;
    void operator=(const AudioManager& manager) = delete;

private:
    // Pointer to the mixer
    MIX_Mixer* m_mixer;

    // Hash table that maps all the loaded audio data to a string name
    std::unordered_map<std::string, MIX_Audio*> m_registry;
};



// ******************
// FONT MANAGER CLASS
// ******************

/**
 * Class for storing and managing font data
 */
class FontManager final
{
public:
    // IMPLEMENTATION
    // ==============

    // Default Constructor
    FontManager() = default;

    // Loads fonts associated with the JSON asset and size
    void LoadFont(const Json::Value& asset, uint8_t size);

    // Returns the font with the specified name and point size
    const TTF_Font* GetFont(const std::string& name) const;

    // Frees all font data loaded by the manager
    ~FontManager();

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and assignment operator have been disabled to
    //       prevent the copying of font assets.
    FontManager(const FontManager& manager) = delete;
    void operator=(const FontManager& manager) = delete;

private:
    // Hash table that maps all the loaded TTF font data to a string name and
    // font point size
    std::unordered_map<std::string, TTF_Font*> m_registry;
};



// *********************
// TEXTURE MANAGER CLASS
// *********************

/**
 *  Class for managing the creation and destruction of SDL textures
 */
class TextureManager final
{
public:
    // IMPLEMENTATION
    // ==============

    // Main Constructor
    //
    // Note: The renderer must already be initialized
    TextureManager(const Renderer& renderer);

    // Loads textures from the associated JSON asset
    void LoadTexture(const Json::Value& asset);

    // Returns the texture with the specified file path
    const SDL_Texture* GetTexture(const std::string& name) const;

    // Destroys all texture data that is loaded by the manager
    ~TextureManager();

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and the assignment operator have been deleted to
    //       prevent a copy of the textures from being created.
    TextureManager(const TextureManager&) = delete;
    void operator=(const TextureManager&) = delete;

private:
    // Pointer to the SDL renderer context that loads the textures
    //
    // Note: The renderer cannot be modified by this class.
    const SDL_Renderer* m_rendererContext;

    // Hash table of all the texture data that are loaded by the renderer mapped to
    // a string name
    std::unordered_map<std::string, SDL_Texture*> m_registry;
};



// *******************
// ASSET MANAGER CLASS
// *******************

/**
 *  Class for managing the creation and destruction of all game assets
 */
class AssetManager
{
public:
    // IMPLEMENTATION
    // ==============

    // Constructor
    AssetManager(const Renderer& renderer);

    // Loads audio data with the associated JSON asset
    void LoadAudio(const Json::Value& asset);

    // Loads font with the associated JSON asset and font size
    void LoadFont(const Json::Value& asset, uint8_t size);

    // Loads texture with the associated JSON asset
    void LoadTexture(const Json::Value& asset);

    // Returns the Audio object with the specified namee
    Audio GetAudio(const std::string& name) const;

    // Returns the font with the specified name
    const TTF_Font* GetFont(const std::string& name) const;

    // Returns the texture with the specified name
    const SDL_Texture* GetTexture(const std::string& name) const;

private:
    // The audio manager
    AudioManager m_audioManager;

    // The font manager
    FontManager m_fontManager;

    // The texture manager
    TextureManager m_textureManager;
};
