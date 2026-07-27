#include <json/value.h>               // Json::Value class
#include <SDL3/SDL_events.h>          // SDL_Event union
#include <SDL3/SDL_mouse.h>           // SDL_GetMouseState function

#include <string>                     // std::string
#include <utility>                    // std::move

#include "Media/Asset.hpp"            // AssetManager class
#include "Media/Window.hpp"           // Window class
#include "UI/Menu.hpp"                // Menu class
#include "Utility/Assert.hpp"         // GAME_2D_ASSERT macro function
#include "Utility/Log.hpp"            // GAME_2D_LOG_ERROR macro function


// ******************
// SYMBOLIC CONSTANTS
// ******************

static constexpr uint8_t MAX_WIDGETS = 10;



// **************
// IMPLEMENTATION
// **************

Menu::Menu():
    m_parent (nullptr)
{
    m_widgets.reserve(MAX_WIDGETS);
}



Menu::Menu(const Window& window):
    m_parent (&window)
{
    m_widgets.reserve(MAX_WIDGETS);
}



void Menu::SetParentWindow(const Window& window)
{
    m_parent = &window;
}



void Menu::CreateWidgets(const AssetManager& manager, const Json::Value& widgets)
{
    GAME_2D_ASSERT(m_parent);

    for (const Json::Value& widget : widgets)
    {
        std::string type = widget["type"].asCString();

        if (type == "text")
            AddText(manager, widget);

        else if (type == "button")
            AddButton(manager, widget);

        else if (type == "slider")
            AddSlider(manager, widget);

        else
            GAME_2D_LOG_ERROR("Invalid widget type\n");
    }

    GAME_2D_ASSERT(m_widgets.size() <= MAX_WIDGETS);
}



const std::vector<std::unique_ptr<Widget>>& Menu::GetWidgets() const
{
    return m_widgets;
}



void Menu::HandleInput(const SDL_Event& event)
{
    for (auto& widget : m_widgets)
        widget->HandleInput(event);
}



void Menu::Update()
{
    for (auto& widget : m_widgets)
        widget->Update();
}



// ******************
// INTERNAL FUNCTIONS
// ******************

void Menu::AddText(const AssetManager& manager, const Json::Value& widget)
{
    std::unique_ptr text = std::make_unique<Text>(*m_parent);

    const char* label = widget["label"].asCString();
    const char* font  = widget["font"].asCString();
    const float normX = widget["coord"][0].asFloat();
    const float normY = widget["coord"][1].asFloat();

    text->str     = widget["string"].asCString();
    text->font    = manager.GetFont(font);
    text->color.r = widget["color"][0].asInt();
    text->color.g = widget["color"][1].asInt();
    text->color.b = widget["color"][2].asInt();

    text->SetLabel(label);
    text->SetPosition(normX, normY);

    m_widgets.emplace_back( std::move(text) );
}



void Menu::AddButton(const AssetManager& manager, const Json::Value& widget)
{
    std::unique_ptr button = std::make_unique<Button>(*m_parent);

    const char* label = widget["label"].asCString();
    const char* icon  = widget["icon"].asCString();
    const float normX = widget["coord"][0].asFloat();
    const float normY = widget["coord"][1].asFloat();

    button->icon = manager.GetTexture(icon);

    button->SetLabel(label);
    button->SetPosition(normX, normY);

    m_widgets.emplace_back( std::move(button) );
}



void Menu::AddSlider(const AssetManager& manager, const Json::Value& widget)
{
    std::unique_ptr slider = std::make_unique<Slider>(*m_parent);

    const char* label = widget["label"].asCString();
    const char* track = widget["track"].asCString();
    const char* knob  = widget["knob"].asCString();
    const float normX = widget["coord"][0].asFloat();
    const float normY = widget["coord"][1].asFloat();

    slider->trackIcon = manager.GetTexture(track);
    slider->knobIcon  = manager.GetTexture(knob);

    slider->SetLabel(label);
    slider->SetPosition(normX, normY);

    m_widgets.emplace_back( std::move(slider) );
}
