#pragma once

#include <SDL2/SDL_pixels.h>    // SDL_Color struct

#include <map>                  // std::map
#include <string>               // std::string
#include <string_view>          // std::string_view

struct _TTF_Font;               // typedef struct _TTF_Font TTF_Font


/**
 * Struct for storing screen text data
 */
struct Text
{
    std::string str   = "";             // The text characters
    _TTF_Font*  font  = nullptr;        // Pointer to text font data
    SDL_Color   color = { 0, 0, 0 };    // The text color
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
    void LoadFont(std::string_view filepath, uint8_t ptSize);

    // Returns the font with the specified file path and point size
    _TTF_Font* Get(std::string_view filepath, uint8_t ptSize);

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
    // Key for loaded font data which represents the file path and font point size
    using FontKey = std::pair<std::string, uint8_t>;

    // Data struture that maps all the loaded TTF font data to its file path and
    // font point size
    //
    // Note: The destructor automatically destroys all loaded textures. This is not
    //       handled by the main game class.
    std::map<FontKey, _TTF_Font*> m_fonts;
};
