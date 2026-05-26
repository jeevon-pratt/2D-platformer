#pragma once

#include <string>           // std::string
#include <string_view>      // std::string_view
#include <unordered_map>    // std::unordered_map

struct SDL_Renderer;
struct SDL_Texture;

class Renderer;


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

    // Loads textures from the specified file path
    void LoadTexture(std::string_view filepath);

    // Returns the texture with the specified file path
    SDL_Texture* Get(std::string_view filepath);

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
    // their file paths
    //
    // Note: The destructor automatically destroys all loaded textures. This is not
    //       handled by any of the entity classes or the main game class.
    std::unordered_map<std::string, SDL_Texture*> m_textures;
};
