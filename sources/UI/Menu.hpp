#pragma once

#include <memory>           // std::unique_ptr
#include <string>           // std::string
#include <vector>           // std::vector

#include "UI/Widget.hpp"    // Widget class

union  SDL_Event;
struct b2Vec2;
class  AssetManager;
class  Window;

namespace Json
{
    class Value;
}


/**
 * Class for implementing game menus
 */
class Menu final
{
public:
    // IMPLEMENTATION
    // ==============

    // Default Constructor
    Menu();

    // Parent Window Constructor
    Menu(const Window& window);

    // Sets the parent window after initialization
    void SetParentWindow(const Window& window);

    // Creates the widgets of the menu
    void CreateWidgets(const AssetManager& manager, const Json::Value& widgets);

    // Returns the menu widgets
    const std::vector<std::unique_ptr<Widget>>& GetWidgets() const;

    // Handles user input on the menu widgets
    void HandleInput(const SDL_Event& event);

    // Updates the menu widgets
    void Update();

private:
    // Adds a text widget to the menu
    void AddText(const AssetManager& manager, const Json::Value& widget);

    // Adds a button widget to the menu
    void AddButton(const AssetManager& manager, const Json::Value& widget);

    // Adds a slider widget to the menu
    void AddSlider(const AssetManager& manager, const Json::Value& widget);

private:
    // The menu widgets
    std::vector<std::unique_ptr<Widget>> m_widgets;

    // Pointer to the parent window of the menu
    const Window* m_parent;
};
