#include <SDL2/SDL_error.h>             // SDL_GetError function
#include <SDL2/SDL_image.h>             // IMG_LoadTexture and IMG_GetError functions
#include <SDL2/SDL_render.h>            // SDL renderer functions
#include <SDL2/SDL_surface.h>           // SDL_FreeSurface function
#include <SDL2/SDL_ttf.h>               // TTF_Font struct

#include <cmath>                        // M_PI
#include <cstdlib>                      // std::exit, EXIT_FAILURE

#include "Entity/Game_Object.hpp"       // GameObject class
#include "Media/Renderer.hpp"           // Renderer class
#include "Media/Sprite.hpp"             // Sprite class
#include "Media/Text.hpp"               // Text class
#include "Media/User_Interface.hpp"     // UserInterface class
#include "Media/Window.hpp"             // Window class
#include "Utility/Log.hpp"              // GAME_2D_LOG_CRITICAL macro function
#include "Utility/Math.hpp"             // RoundToInt and ConvertToScreenCoord functions


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr int16_t  DRIVER_INDEX = -1;
static constexpr uint32_t INIT_FLAGS   = SDL_RENDERER_PRESENTVSYNC;



// **************
// IMPLEMENTATION
// **************

Renderer::Renderer(const Window& window):
    m_rendererContext ( SDL_CreateRenderer((SDL_Window*)window.GetContext(), DRIVER_INDEX, INIT_FLAGS) )
{
    if (!m_rendererContext)
    {
        GAME_2D_LOG_CRITICAL("%s\n\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }
}



const SDL_Renderer* Renderer::GetContext() const
{
    return m_rendererContext;
}



void Renderer::Clear() const
{
    SDL_RenderClear(m_rendererContext);
}



void Renderer::DrawGradient(SDL_Color color1, SDL_Color color2, float steps) const
{
    int winWidth  = 0;
    int winHeight = 0;

    SDL_GetRendererOutputSize(m_rendererContext, &winWidth, &winHeight);


    float yStep = winHeight / steps;
    float rStep = (color2.r - color1.r) / steps;
    float gStep = (color2.g - color1.g) / steps;
    float bStep = (color2.b - color1.b) / steps;

    SDL_Rect dstrect;
    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = winWidth;
    dstrect.h = RoundToInt<int16_t>(yStep);


    for (uint64_t count = 0; count <= steps; ++count)
    {
        SDL_SetRenderDrawColor(m_rendererContext, color1.r, color1.g, color1.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(m_rendererContext, &dstrect);

        dstrect.y += RoundToInt<int16_t>(yStep);
        color1.r +=  RoundToInt<int16_t>(rStep);
        color1.g +=  RoundToInt<int16_t>(gStep);
        color1.b +=  RoundToInt<int16_t>(bStep);
    }
}



void Renderer::Render(const Sprite& sprite, b2Vec2 pos, b2Vec2 camTransform) const
{
    b2Vec2 screenCoord = pos;

    if (!sprite.UseScreenCoord())
        screenCoord = ConvertToScreenCoord(sprite, pos, camTransform);

    SDL_Rect dstrect;
    dstrect.x = RoundToInt<int16_t>(screenCoord.x);
    dstrect.y = RoundToInt<int16_t>(screenCoord.y);
    dstrect.w = sprite.GetFrameWidth();
    dstrect.h = sprite.GetFrameHeight();


    SDL_RenderCopy(m_rendererContext,
                   const_cast<SDL_Texture*>(sprite.GetTexture()),
                   &sprite.GetSourceRect(),
                   &dstrect);

    // Note: const_cast is used on the object texture because a constant Sprite instance is passed
    //       into the Render function but the 'SDL_RenderCopyEx' function requires a non-const
    //       SDL_Texture pointer.
}



void Renderer::Render(const GameObject& object, b2Vec2 camTransform) const
{
    b2Vec2 screenCoord = ConvertToScreenCoord(object.GetSprite(), object.GetPosition(), camTransform);

    SDL_Rect dstrect;
    dstrect.x = RoundToInt<int16_t>(screenCoord.x);
    dstrect.y = RoundToInt<int16_t>(screenCoord.y);
    dstrect.w = object.GetSprite().GetFrameWidth();
    dstrect.h = object.GetSprite().GetFrameHeight();


    SDL_RenderCopyEx(m_rendererContext,
                     const_cast<SDL_Texture*>( object.GetSprite().GetTexture() ),
                     &object.GetSprite().GetSourceRect(),
                     &dstrect,
                     -object.GetAngle() * ( 180.0 / static_cast<double>(M_PI) ),
                     nullptr,
                     object.IsInverted() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

    // Note: 1) const_cast is used on the object texture because a constant Sprite instance is passed
    //          into the Render function but the 'SDL_RenderCopyEx' function requires a non-const
    //          SDL_Texture pointer.
    //
    //       2) The angle is negated due to the inversion of the y-axis by 'ConvertToScreenCoord'.
}



void Renderer::Render(const Text& text, b2Vec2 screenCoord) const
{
    TTF_Font* font = const_cast<TTF_Font*>(text.font);

    SDL_Surface* surfaceData = TTF_RenderText_Solid(font, text.str.data(), text.color);
    SDL_Texture* textureData = SDL_CreateTextureFromSurface(m_rendererContext, surfaceData);

    SDL_Rect dstrect;
    dstrect.x = RoundToInt<int16_t>(screenCoord.x);
    dstrect.y = RoundToInt<int16_t>(screenCoord.y);
    dstrect.w = (surfaceData ? surfaceData->w : 0);
    dstrect.h = (surfaceData ? surfaceData->h : 0);


    SDL_RenderCopy(m_rendererContext, textureData, nullptr, &dstrect);
    SDL_FreeSurface(surfaceData);
    SDL_DestroyTexture(textureData);

    // Note: const_cast is used because a constant Text instance is passed into the Render function
    //       but the' TTF_RenderText_Solid' function requires a non-const TTF_Font pointer.
}



void Renderer::Render(const UserInterface& interface) const
{
    Render(interface.GetBackground(), b2Vec2(0.0f, 0.0f));

    for (const Button& button : interface.GetButtons())
        Render(button.image, button.pos);
}



void Renderer::Display() const
{
    SDL_RenderPresent(m_rendererContext);
}



Renderer::~Renderer()
{
    SDL_DestroyRenderer(m_rendererContext);
}
