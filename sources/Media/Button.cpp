#include <SDL3/SDL_events.h>    // SDL_Event union, SDL_EVENT_MOUSE_BUTTON_DOWN event

#include "Media/Button.hpp"     // Button class
#include "Utility/Math.hpp"     // RoundToInt and InBounds functions


// **************
// IMPLEMENTATION
// **************

bool Button::IsActive(float mouseX, float mouseY) const
{
    float x1 = pos.x;
    float y1 = pos.y;

    float x2 = pos.x + image.GetFrameWidth();
    float y2 = pos.y + image.GetFrameHeight();


    return InBounds(mouseX, x1, x2) && InBounds(mouseY, y1, y2);
}



bool Button::IsClicked(float mouseX, float mouseY, const SDL_Event& event) const
{
    return IsActive(mouseX, mouseY) && (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
}
