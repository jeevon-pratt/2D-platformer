#include <SDL3/SDL_events.h>    // SDL_Event union, SDL_PushEvent function
#include <SDL3/SDL_render.h>    // SDL_GetTextureSize function

#include <algorithm>            // std::clamp

#include "Media/Renderer.hpp"   // Renderer class
#include "Media/Window.hpp"     // Window class
#include "UI/Widget.hpp"        // Slider class
#include "Utility/Math.hpp"     // InBounds functions


// **************
// IMPLEMENTATION
// **************

Slider::Slider():
    Widget    (),
    trackIcon (nullptr),
    knobIcon  (nullptr),
    m_active  (false),
    m_clicked (false),
    m_value   (0.0f)
{
}



Slider::Slider(const Window& window):
    Widget    (window),
    trackIcon (nullptr),
    knobIcon  (nullptr),
    m_active  (false),
    m_clicked (false),
    m_value   (0.0f)
{
}



float Slider::GetWidth() const
{
    float width = 0.0f;

    if (!trackIcon)
        return width;

    SDL_Texture* trackIcon = const_cast<SDL_Texture*>(this->trackIcon);

    SDL_GetTextureSize(trackIcon, &width, nullptr);

    return width;
}



float Slider::GetHeight() const
{
    float height = 0.0f;

    if (!trackIcon)
        return height;

    SDL_Texture* trackIcon = const_cast<SDL_Texture*>(this->trackIcon);

    SDL_GetTextureSize(trackIcon, nullptr, &height);

    return height;
}



float Slider::GetKnobWidth() const
{
    float width = 0.0f;

    if (!knobIcon)
        return width;

    SDL_Texture* knobIcon = const_cast<SDL_Texture*>(this->knobIcon);

    SDL_GetTextureSize(knobIcon, &width, nullptr);

    return width;
}



float Slider::GetKnobHeight() const
{
    float height = 0.0f;

    if (!knobIcon)
        return height;

    SDL_Texture* knobIcon = const_cast<SDL_Texture*>(this->knobIcon);

    SDL_GetTextureSize(knobIcon, nullptr, &height);

    return height;
}



b2Vec2 Slider::GetKnobPosition() const
{
    float height    = GetKnobHeight();
    float winHeight = m_parent->GetHeight();

    float begin = GetTrackBegin();
    float end   = GetTrackEnd();

    b2Vec2 position;
    position.x = begin + m_value * (end - begin);
    position.y = (m_coord.y * winHeight) - (height / 2.0f);

    return position;
}



void Slider::HandleInput(const SDL_Event& event)
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

        if (!m_clicked)
            return;

        m_clicked = false;
        break;


    case SDL_EVENT_MOUSE_MOTION:

        if (!m_clicked)
            return;

        MoveKnob(event.motion.x);
        PushSliderEvent();
        break;
    }
}



void Slider::Update()
{
    float mouseX;
    float mouseY;

    SDL_GetMouseState(&mouseX, &mouseY);


    auto [x1, y1] = GetPosition();

    float x2 = (x1 + GetWidth());
    float y2 = (y1 + GetHeight());

    m_active = InBounds(mouseX, x1, x2) && InBounds(mouseY, y1, y2);
}



void Slider::OnRender(Renderer& renderer) const
{
    renderer.Render(*this);
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void Slider::PushSliderEvent()
{
    SDL_Event event;
    SDL_zero(event);

    event.type       = Slider::Event;
    event.user.data1 = static_cast<void*>(m_label.data());
    event.user.data2 = static_cast<void*>(&m_value);

    SDL_PushEvent(&event);
}



void Slider::MoveKnob(float mouseX)
{
    float begin     = GetTrackBegin();
    float end       = GetTrackEnd();
    float knobWidth = GetKnobWidth();
    float cursorPos = mouseX - (knobWidth / 2.0f);

    m_value = (cursorPos - begin) / (end - begin);
    m_value = std::clamp(m_value, 0.0f, 1.0f);
}



float Slider::GetTrackBegin() const
{
    float winWidth = m_parent->GetWidth();
    float width    = GetWidth();

    return (m_coord.x * winWidth) - (width / 2.0f);
}



float Slider::GetTrackEnd() const
{
    float winWidth  = m_parent->GetWidth();
    float width     = GetWidth();
    float knobWidth = GetKnobWidth();

    return (m_coord.x * winWidth) + (width / 2.0f) - knobWidth;
}
