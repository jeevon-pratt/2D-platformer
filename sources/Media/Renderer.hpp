#pragma once

#include <box2d/b2_math.h>    // b2Vec2 struct

#include <unordered_map>      // std::unordered_map

struct SDL_Color;
struct SDL_Renderer;
struct SDL_Window;
union  SDL_Event;
class  Button;
class  GameObject;
class  Menu;
class  PerfMonitor;
class  Slider;
class  Sprite;
class  Text;
class  Window;


// *******************
// TEXT RENDERER CLASS
// *******************

/**
 * Renderer for Text Objects
 */
class TextRenderer final
{
public:
    // IMPLEMENTATION
    // ==============

    // The Default Constructor
    TextRenderer();

    // Renders the text and caches new text draw data
    void Render(SDL_Renderer* renderer, const Text& text);

    // Destroys the cached text draw data
    ~TextRenderer();

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and assignment operator have been disabled to
    //       prevent the copying of text draw data.
    TextRenderer(const TextRenderer& renderer) = delete;
    void operator=(const TextRenderer& renderer) = delete;

    // Stores text texture in the text cache
    void CacheText(SDL_Renderer* renderer, const Text& text);

    // Destroys the text texture in the text cache
    void FreeCache();

private:
    // NESTED CLASSES
    // ==============

    // Custon hasher for text cache
    struct TextHasher
    {
        size_t operator()(const Text& text) const noexcept;
    };

    // Custom equality functor for text cache
    struct TextEqual
    {
        bool operator()(const Text& textA, const Text& textB) const;
    };

private:
    // The cache of text textures
    std::unordered_map<Text, SDL_Texture*, TextHasher, TextEqual> m_cache;
};



// **************
// RENDERER CLASS
// **************

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

    // Handles user input for the ImGui context
    void HandleInput(const SDL_Event& event);

    // Clears the internal SDL renderer
    void Clear();

    // Draws a color gradient onto the screen
    void DrawGradient(SDL_Color color1, SDL_Color color2, float steps);

    // Renders a sprite to the screen with the specified coordinates and optional camera transform
    void Render(const Sprite& sprite, b2Vec2 pos, b2Vec2 cameraTransform = b2Vec2(0.0f, 0.0f));

    // Renders a game object to the screen with an optional camera transform
    void Render(const GameObject& object, b2Vec2 cameraTransform = b2Vec2 (0.0f, 0.0f));

    // Renders text
    void Render(const Text& text);

    // Renders a button
    void Render(const Button& button);

    // Renders a slider
    void Render(const Slider& slider);

    // Renders the widgets of a menu
    void Render(const Menu& menu);

    // Renders the performance monitor window
    void Render(const PerfMonitor& monitor);

    // Displays the rendered objects onto the screen
    void Display();

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

    // Text renderer
    TextRenderer m_textRenderer;
};
