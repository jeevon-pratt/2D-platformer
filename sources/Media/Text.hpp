#pragma once

#include <SDL2/SDL_pixels.h>    // SDL_Color struct

#include <string>               // std::string
#include <string_view>          // std::string_view
#include <unordered_map>        // std::unordered_map

struct _TTF_Font;               // typedef struct _TTF_Font TTF_Font


/**
 * Struct for storing screen text data
 */
struct Text
{
    std::string      str   = "";             // The text characters
    const _TTF_Font* font  = nullptr;        // Pointer to text font data
    SDL_Color        color = { 0, 0, 0 };    // The text color
};




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

    // Loads fonts with the specified .ttf file path and point size
    void LoadFont(std::string_view name, std::string_view filepath, uint8_t size);

    // Returns the font with the specified name and point size
    const _TTF_Font* Get(std::string_view name) const;

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
    //
    // Note: The destructor automatically destroys all loaded textures. This is not
    //       handled by the main game class.
    std::unordered_map<std::string, const _TTF_Font*> m_fonts;
};
