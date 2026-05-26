#pragma once

#include <string_view>      // std::string_view

union  SDl_Event;
struct SDL_Window;


/**
 *  Wrapper around the SDL window structure
 */
class Window final
{
public:
    // IMPLEMENTATION
    // ==============

    // Main Constructor
    Window(std::string_view title, uint16_t width, uint16_t height);

    // Returns the SDl window context
    const SDL_Window* GetContext() const;

    // Returns the window width in pixels
    uint16_t GetWidth() const;

    // Returns the window height in pixels
    uint16_t GetHeight() const;

    // Handles User input on SDL_Window
    void HandleInput(const SDL_Event& event) const;

    // Shows a pop-up window with a message
    void ShowMessageBox(std::string_view title, std::string_view message) const;

    // Destroys the internal SDL_Window
    ~Window();

public:
    // STATIC FUNCTIONS
    // ================

    // Enables the cursor on the window
    static void ShowCursor();

    // Disables the cursor on the window
    static void HideCursor();

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and the assignment operator have been deleted to prevent a
    //       copy of the window context from being created.
    Window(const Window&) = delete;
    void operator=(const Window&) = delete;

private:
    // The internal SDL_window context to which game sprites are displayed
    //
    // Note: The context can be modified even if the Window object is const.
    SDL_Window* m_windowContext;
};
