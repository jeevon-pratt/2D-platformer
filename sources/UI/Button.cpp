#include <SDL3/SDL_events.h>    // SDL_Event union macro, SDL_PushEvent function
#include <SDL3/SDL_render.h>    // SDL_GetTextureSize, SDL_SetTextureColorMod functions

#include "Media/Renderer.hpp"   // Renderer class
#include "Media/Window.hpp"     // Window class
#include "UI/Widget.hpp"        // Button class
#include "Utility/Math.hpp"     // InBounds function


// **************
// IMPLEMENTATION
// **************

Button::Button():
    Widget     (),
    icon       (nullptr),
    m_colorMod {255, 255, 255},
    m_active   (false),
    m_clicked  (false)
{
}



Button::Button(const Window& window):
    Widget     (window),
    icon       (nullptr),
    m_colorMod {255, 255, 255},
    m_active   (false),
    m_clicked  (false)
{
}



float Button::GetWidth() const
{
    float width = 0.0f;

    if (!icon)
        return width;

    SDL_Texture* icon = const_cast<SDL_Texture*>(this->icon);

    SDL_GetTextureSize(icon, &width, nullptr);

    return width;
}



float Button::GetHeight() const
{
    float height = 0.0f;

    if (!icon)
        return height;

    SDL_Texture* icon = const_cast<SDL_Texture*>(this->icon);

    SDL_GetTextureSize(icon, nullptr, &height);

    return height;
}



SDL_Color Button::GetColorMod() const
{
    return m_colorMod;
}



void Button::HandleInput(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:

        if (event.button.button != SDL_BUTTON_LEFT)
            return;

        if (!m_active)
            return;

        m_clicked = true;
        break;


    case SDL_EVENT_MOUSE_BUTTON_UP:

        if (event.button.button != SDL_BUTTON_LEFT)
            return;

        if (!m_active || !m_clicked)
            return;

        m_clicked = false;
        
        PushButtonEvent();
        break;
    }
}



void Button::Update()
{
    float mouseX;
    float mouseY;

    SDL_GetMouseState(&mouseX, &mouseY);


    auto [x1, y1] = GetPosition();

    float x2 = (x1 + GetWidth());
    float y2 = (y1 + GetHeight());


    if ( InBounds(mouseX, x1, x2) && InBounds(mouseY, y1, y2) )
    {
        m_active     = true;
        m_colorMod.r = 200;
        m_colorMod.g = 200;
        m_colorMod.b = 200;
    }

    else if (m_active)
    {
        m_active     = false;
        m_colorMod.r = 255;
        m_colorMod.g = 255;
        m_colorMod.b = 255;
    }
}



void Button::OnRender(Renderer& renderer) const
{
    renderer.Render(*this);
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void Button::PushButtonEvent()
{
    SDL_Event event;
    SDL_zero(event);

    event.type       = Button::Event;
    event.user.data1 = static_cast<void*>(m_label.data());
    event.user.data2 = nullptr;

    SDL_PushEvent(&event);
}
