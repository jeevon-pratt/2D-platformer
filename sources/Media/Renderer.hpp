#pragma once

#include <box2d/b2_math.h>  // b2Vec2 class

struct b2Vec2;

struct SDL_Color;
struct SDL_Renderer;
struct SDL_Window;

class  GameObject;
class  Sprite;
class  UserInterface;
class  Window;
struct Text;


/**
 *  Wrapper around the SDL renderer structure
 */
class Renderer final
{
public:
    // SYMBOLIC CONSTANTS
    // ==================

    // Pixels-to-meter conversion factor (64 pixels = 1 game world meter)
    static constexpr float PIXELS_PER_METER = 64.0f;

public:
    // IMPLEMENTATION
    // ==============

    // Main Constructor
    //
    // Note: The window must already be initialized.
    Renderer(const Window& window);

    // Returns the SDL renderer context
    const SDL_Renderer* GetContext() const;

    // Clears the internal SDL renderer
    void Clear() const;

    // Draws a color gradient onto the screen
    void DrawGradient(SDL_Color color1, SDL_Color color2, float steps) const;

    // Renders a sprite to the screen with the specified coordinates and optional camera transform
    void Render(const Sprite& sprite, b2Vec2 pos, b2Vec2 cameraTransform = b2Vec2(0.0f, 0.0f)) const;

    // Renders a game object to the screen with an optional camera transform
    void Render(const GameObject& object, b2Vec2 cameraTransform = b2Vec2 (0.0f, 0.0f)) const;

    // Renders text to the screen with the specified coordinates
    void Render(const Text& text, b2Vec2 screenCoord) const;

    // Renders the elements of a user interface
    void Render(const UserInterface& interface) const;

    // Displays the rendered objects onto the screen
    void Display() const;

    // Destroys the internal SDL renderer
    ~Renderer();

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and the assignment operator have been deleted to
    //       prevent a copy of the rendering context from being created.
    Renderer(const Renderer&) = delete;
    void operator=(const Renderer&) = delete;

private:
    // The internal SDL renderer context responsible for drawing sprites
    //
    // Note: The context can be modified even if the Renderer object is const.
    SDL_Renderer* m_rendererContext;
};
