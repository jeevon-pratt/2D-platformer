#pragma once

#include <box2d/b2_math.h>      // b2Vec2 class

#include <string>               // std::string

#include "Media/Sprite.hpp"     // Sprite class

union SDL_Event;


/**
 * Class for implementing button functionality
 */
class Button
{
public:
    // IMPLEMENTATION
    // ==============

    // Default constructor
    Button() = default;

    // Determines if the mouse is currently hovering over the button
    bool IsActive(uint16_t mouseX, uint16_t mouseY) const;

    // Determines if the button has been clicked
    bool IsClicked(uint16_t mouseX, uint16_t mouseY, const SDL_Event& event) const;

public:
    // The name of the button
    std::string name;

    // The icon image of the button
    Sprite image;

    // The screen coordinate of the button
    b2Vec2 pos;
};
