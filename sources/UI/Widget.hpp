#pragma once

#include <box2d/b2_math.h>      // b2Vec2 class
#include <SDL3/SDL_events.h>    // SDL_EVENT_USER macro
#include <SDL3/SDL_pixels.h>    // SDL_Color struct
#include <SDL3_ttf/SDL_ttf.h>   // TTF_Font struct

#include <string>               // std::string

union  SDL_Event;
struct SDL_Texture;
class  Renderer;
class  Window;


// **********************
// WIDGET INTERFACE CLASS
// **********************

/**
 * Class for implementing widgets
 */
class Widget
{
public:
    // IMPLEMENTATION
    // ==============

    // Default Constructor
    Widget();

    // Parent Window Constructor
    Widget(const Window& window);

    // Sets the widget label
    virtual void SetLabel(const std::string& label);

    // Sets the parent window
    virtual void SetParentWindow(const Window& window);

    // Sets the position of the widget on the parent window
    //
    // Note: 1) Should only be called after setting the parent window
    //
    //       2) This function should be provided with normalized
    //          coordinates that are between 0 and 1
    virtual void SetPosition(float normX, float normY);

    // Returns the widget width
    virtual float GetWidth() const = 0;

    // Returns the widget height
    virtual float GetHeight() const = 0;

    // Returns the screen position of the widget
    virtual b2Vec2 GetPosition() const;

    // Handles user input
    virtual void HandleInput(const SDL_Event& event) = 0;

    // Updates the widget
    virtual void Update() = 0;

    // Accepts a Renderer visitor
    virtual void OnRender(Renderer& renderer) const = 0;

    // Default Destructor
    virtual ~Widget() = default;
    
protected:
    // The string label of the button
    std::string m_label;

    // Pointer to the parent window of the widget
    const Window* m_parent;

    // The normalized screen coordinates of the widget
    b2Vec2 m_coord;
};



// **********
// TEXT CLASS
// **********

/**
 * Struct for storing screen text data
 */
class Text final : public Widget
{
public:
    // IMPLMENETATION
    // ==============

    // Default Constructor
    Text();

    // Parent Window Constructor
    Text(const Window& window);

    // Returns the width of the sprite texture
    //
    // Note: This function should only be called after setting the text
    //       string and the text font
    virtual float GetWidth() const override;

    // Returns the height of the sprite texture
    //
    // Note: This function should only be called after setting the text
    //       string and the text font
    virtual float GetHeight() const override;

      // Handles user input
    virtual void HandleInput(const SDL_Event& event) override;

    // Updates the text
    virtual void Update() override;

    // Accepts a Renderer visitor
    virtual void OnRender(Renderer& renderer) const override;

    // Default Destructor
    virtual ~Text() = default;

public:
    // The text characters
    std::string str;

    // Pointer to text font data
    const TTF_Font* font;

    // The text color
    SDL_Color color;
};



// ************
// BUTTON CLASS
// ************

/**
 * Class for implementing buttons
 */
class Button final : public Widget
{
public:
    // Custom button released event
    static constexpr uint32_t Event = SDL_EVENT_USER;

public:
    // IMPLEMENTATION
    // ==============

    // Default Constructor
    Button();

    // Parent Window Constructor
    Button(const Window& window);

    // Returns the button width
    virtual float GetWidth() const override;

    // Returns the button height
    virtual float GetHeight() const override;

    // Returns the button color mod
    SDL_Color GetColorMod() const;

    // Handles user input
    virtual void HandleInput(const SDL_Event& event) override;

    // Updates the button
    virtual void Update() override;

    // Accepts a Renderer visitor
    virtual void OnRender(Renderer& renderer) const override;

    // Default Destructor
    virtual ~Button() = default;

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Pushes button event
    void PushButtonEvent();

public:
    // The icon texture of the button
    const SDL_Texture* icon;

private:
    // Color mod for button icon
    SDL_Color m_colorMod;

    // Boolean indicating whether the button is active
    bool m_active;

    // Boolean indicating whether the button is clicked
    bool m_clicked;
};



// ************
// SLIDER CLASS
// ************

/**
 * Class for implementing buttons
 */
class Slider final : public Widget
{
public:
    // Custom button released event
    static constexpr uint32_t Event = (SDL_EVENT_USER + 1);

public:
    // IMPLEMENTATION
    // ==============

    // Default Constructor
    Slider();

    // Parent Window Constructor
    Slider(const Window& window);

    // Returns the slider track width
    virtual float GetWidth() const override;

    // Returns the slider track height
    virtual float GetHeight() const override;

    // Returns the slider knob width
    float GetKnobWidth() const;

    // Returns the slider knob height
    float GetKnobHeight() const;

    // Returns the slider knob position
    b2Vec2 GetKnobPosition() const;

    // Handles user input
    virtual void HandleInput(const SDL_Event& event) override;

    // Updates the button
    virtual void Update() override;

    // Accepts a Renderer visitor
    virtual void OnRender(Renderer& renderer) const override;

    // Default Destructor
    virtual ~Slider() = default;

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Pushes slider event
    void PushSliderEvent();

    // Updates the slider knob value
    void MoveKnob(float mouseX);

    // Returns the slider track begin position
    float GetTrackBegin() const;

    // Returns the slider track end position
    float GetTrackEnd() const;

public:
    // The texture for the slider track
    const SDL_Texture* trackIcon;

    // The texture for the slider knob
    const SDL_Texture* knobIcon;

private:
    // Boolean indicating whether the knob is active
    bool m_active;

    // Boolean indicating whether the knob is clicked
    bool m_clicked;

    // Slider knob percentage value
    float m_value;
};
