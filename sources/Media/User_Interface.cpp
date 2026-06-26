#include <SDL3/SDL_events.h>            // SDL_Event union
#include <SDL3/SDL_mouse.h>             // SDL_GetMouseState function

#include "Media/User_Interface.hpp"     // UserInterface class


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr uint8_t MAX_UI_BUTTONS = 10;



// **************
// IMPLEMENTATION
// **************

UserInterface::UserInterface()
{
    m_buttons.reserve(MAX_UI_BUTTONS);

    // To ensure that the iterator is always in a valid state
    m_clickedButton = m_buttons.end();
}



UserInterface::UserInterface(const UserInterface& interface):
    m_background    (interface.m_background),
    m_buttons       (interface.m_buttons),
    m_clickedButton ( m_buttons.end() )
{
}



void UserInterface::operator=(const UserInterface& interface)
{
    m_background    = interface.m_background;
    m_buttons       = interface.m_buttons;
    m_clickedButton = m_buttons.end();
}



void UserInterface::AddBackground(const SpriteCreateInfo& info)
{
    m_background = info;
}



void UserInterface::AddButton(const std::string& name, const Sprite& image, b2Vec2 pos)
{
    m_buttons.emplace_back();

    m_buttons.back().name  = name;
    m_buttons.back().image = image;
    m_buttons.back().pos   = pos;

    // To ensure that the iterator is always in a valid state
    m_clickedButton = m_buttons.end();
}



void UserInterface::HandleInput(const SDL_Event& event)
{
    float mouseX;
    float mouseY;

    SDL_GetMouseState(&mouseX, &mouseY);


    for (auto itr = m_buttons.begin(); itr < m_buttons.end(); ++itr)
    {
        bool buttonActive  = itr->IsActive(mouseX, mouseY);
        bool buttonClicked = itr->IsClicked(mouseX, mouseY, event);


        if (buttonActive)
            itr->image.PlayAnimation("default"); // "hover"

        if (buttonClicked)
        {
            m_clickedButton = itr;
            return;
        }
    }

    m_clickedButton = m_buttons.end();
}



const Sprite& UserInterface::GetBackground() const
{
    return m_background;
}



const std::vector<Button>& UserInterface::GetButtons() const
{
    return m_buttons;
}



bool UserInterface::OnClick(const std::string& name) const
{
    if (m_clickedButton != m_buttons.end())
        return (m_clickedButton->name == name);

    return false;
}
