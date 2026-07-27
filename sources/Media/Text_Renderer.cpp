#include <SDL3/SDL_error.h>     // SDL_GetError function
#include <SDL3/SDL_render.h>    // SDL Renderer functions
#include <SDL3/SDL_surface.h>   // SDL_Surface functions

#include <bitset>               // std::hash

#include "Media/Renderer.hpp"   // TextRenderer class
#include "UI/Widget.hpp"       // Text class
#include "Utility/Log.hpp"      // GAME_2D_LOG_DEBUG macro function


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr uint8_t   MAX_CACHE_SIZE = 50;
static constexpr SDL_Color DEFAULT_COLOR  = {255, 255, 255};


// **************
// IMPLEMENTATION
// **************

TextRenderer::TextRenderer()
{
    m_cache.reserve(MAX_CACHE_SIZE);
}



void TextRenderer::Render(SDL_Renderer* renderer, const Text& text)
{
    if (!renderer)
        return;

    if (!m_cache.contains(text))
         CacheText(renderer, text);

    SDL_Texture* texture = m_cache[text];
        
    SDL_FRect dstrect;
    dstrect.x = text.GetPosition().x;
    dstrect.y = text.GetPosition().y;
    dstrect.w = text.GetWidth();
    dstrect.h = text.GetHeight();
    
    SDL_SetTextureColorMod(texture, text.color.r, text.color.g, text.color.b);
    SDL_RenderTexture(renderer, texture, nullptr, &dstrect);
}



TextRenderer::~TextRenderer()
{
    FreeCache();
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void TextRenderer::CacheText(SDL_Renderer* renderer, const Text& text)
{
    GAME_2D_LOG_DEBUG("Caching text: \"%s\"\n", text.str.c_str());

    if (m_cache.size() > MAX_CACHE_SIZE)
    {
        FreeCache();

        m_cache.clear();
        m_cache.reserve(MAX_CACHE_SIZE);
    }

    TTF_Font*    font    = const_cast<TTF_Font*>(text.font);
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.str.c_str(), 0, DEFAULT_COLOR);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);

    m_cache.emplace(text, texture);
}



void TextRenderer::FreeCache()
{
    GAME_2D_LOG_DEBUG("Clearing cache\n");

    for (auto& [text, texture] : m_cache)
    {
        GAME_2D_LOG_DEBUG("Destroying text: \"%s\"\n", text.str.c_str());
        SDL_DestroyTexture(texture);
    }
}



// **************
// NESTED CLASSES
// **************

size_t TextRenderer::TextHasher::operator()(const Text& text) const noexcept
{
    size_t h1 = std::hash<std::string>()(text.str);
    size_t h2 = std::hash<const TTF_Font*>()(text.font);

    return  h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}



bool TextRenderer::TextEqual::operator()(const Text& textA, const Text& textB) const
{
    return (textA.str == textB.str) && (textA.font == textB.font);
}