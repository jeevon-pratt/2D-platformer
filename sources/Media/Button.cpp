#include <SDL2/SDL_events.h>    // SDL_Event union, SDL_MOUSEBUTTONDOWN event

#include "Media/Button.hpp"     // Button class
#include "Utility/Math.hpp"     // RoundToInt and InBounds functions


// **************
// IMPLEMENTATION
// **************

bool Button::IsActive(uint16_t mouseX, uint16_t mouseY) const
{
    uint16_t x1 = RoundToInt<uint16_t>(pos.x);
    uint16_t y1 = RoundToInt<uint16_t>(pos.y);

    uint16_t x2 = RoundToInt<uint16_t>(pos.x + image.GetFrameWidth());
    uint16_t y2 = RoundToInt<uint16_t>(pos.y + image.GetFrameHeight());


    return InBounds(mouseX, x1, x2) && InBounds(mouseY, y1, y2);
}



bool Button::IsClicked(uint16_t mouseX, uint16_t mouseY, const SDL_Event& event) const
{
    return IsActive(mouseX, mouseY) && (event.type == SDL_MOUSEBUTTONDOWN);
}
