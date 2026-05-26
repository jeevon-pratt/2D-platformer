#pragma once

#include <string_view>          // std::string_view
#include <vector>               // std::vector

#include "Media/Button.hpp"     // Button class
#include "Media/Sprite.hpp"     // Sprite class, SpriteCreateInfo struct

union SDL_Event;

struct b2Vec2;


/**
 * Class for implementing user interface functionality
 */
class UserInterface final
{
public:
    // IMPLEMENTATION
    // ==============

    // Default Constructor
    UserInterface();

    // Copy Constructor
    UserInterface(const UserInterface& interface);

    // Assignment operator
    void operator=(const UserInterface& interface);

    // Creates the background of the interface
    void AddBackground(const SpriteCreateInfo& info);

    // Adds a button to the interface
    void AddButton(std::string_view name, const Sprite& image, b2Vec2 pos);

    // Handles user input on the interface
    void HandleInput(const SDL_Event& event);

    // Returns the interface background
    const Sprite& GetBackground() const;

    // Returns the vector of buttons on the interface
    const std::vector<Button>& GetButtons() const;

    // Checks if a certain button was clicked
    //
    // Note: The 'HandleInput' method must be called before this method in the
    //       game loop. Otherwise, this method will always return false.
    bool OnClick(std::string_view name) const;

private:
    // The background of the interface
    //
    // Note: The background is rendered with screen coordinates. The background should
    //       take up the entire screen as it is automatically rendered from the top left
    //       corner of the screen.
    Sprite m_background;

    // All of the buttons in the interface
    //
    // Note: The button should be rendered using screen coordinates, but the positions
    //       of the buttons can be specified with the 'AddButton' method.
    std::vector<Button> m_buttons;

    // Iterator to the button being clicked by the mouse
    //
    // Note: If the none of the buttons in the interface are active, the iterator points
    //       to the end iterator of the buttons vector.
    std::vector<Button>::const_iterator m_clickedButton;
};
