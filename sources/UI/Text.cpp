#include <SDL3_ttf/SDL_ttf.h>     // TTF_GetStringSize function

#include "Media/Renderer.hpp"     // Renderer class
#include "Media/Window.hpp"       // Window class
#include "UI/Widget.hpp"          // Text class


// **************
// IMPLEMENTATION
// **************

Text::Text() :
    Widget (),
    font   (nullptr),
    color  {0, 0, 0}
{
}



Text::Text(const Window& window) :
    Widget (window),
    font   (nullptr),
    color  {0, 0, 0}
{
}



float Text::GetWidth() const
{
    int width = 0;

    if (str.empty() || !font)
        return width;

    TTF_Font* font = const_cast<TTF_Font*>(this->font);

    TTF_GetStringSize(font, str.c_str(), 0, &width, nullptr);

    return width;
}



float Text::GetHeight() const
{
    int height = 0;

    if (str.empty() || !font)
        return height;

    TTF_Font* font = const_cast<TTF_Font*>(this->font);

    TTF_GetStringSize(font, str.c_str(), 0, nullptr, &height);

    return height;
}



void Text::HandleInput(const SDL_Event& event)
{
    // No functionality
}



void Text::Update()
{
    // No functionality
}



void Text::OnRender(Renderer& renderer) const
{
    renderer.Render(*this);
}