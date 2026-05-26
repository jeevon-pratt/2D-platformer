#pragma once

#include <box2d/b2_body.h>      // b2Vec2 class
#include <json/json.h>          // Json:::Value

#include <string>               // std::string
#include <string_view>          // std::string_view
#include <unordered_map>        // std::unordered_map

#include "Media/Animation.hpp"  // Animation class, Frame class

struct SDL_Texture;
struct SDL_Rect;


/**
 *  Structure for creating sprites
 */
struct SpriteCreateInfo
{
    SDL_Texture* texture        = nullptr;  // The texture of the sprite
    std::string  dataFilePath   = "";       // The file path for the animation meta data
    bool         useScreenCoord = false;    // Default coordinate type is screen coordinates
    float        scrollFactor   = 1.0f;     // Sprite scroll factor for parallax scrolling
};


/**
 *  Wrapper around the SDL_Texture structure
 */
class Sprite final
{
public:
    // IMPLEMENTATION
    // ==============

    // Default Constructor
    Sprite();

    // Constructor using sprite create info
    Sprite(const SpriteCreateInfo& info);

    // Loads the texture meta data from a local file
    //
    // Note: This method is also called in the sprite create info constructor
    void LoadAnimations(std::string_view filepath);

    // Returns the current animation frame of the sprite
    const SDL_Rect& GetSourceRect() const;

    // Returns texture of the sprite
    SDL_Texture* GetTexture() const;

    // Returns sprite frame width in pixels
    uint16_t GetFrameWidth() const;

    // Returns sprite frame height in pixels
    uint16_t GetFrameHeight() const;

    // Returns the coordinate type of the Sprite
    bool UseScreenCoord() const;

    // Returns the scroll factor of the Sprite
    float GetScrollFactor() const;

    // Modifys the alpha value of the texture
    void SetAlphaMod(uint8_t alpha);

    // Plays the animation assigned to a string key
    void PlayAnimation(std::string_view name);

private:
    // A pointer to the texture of the sprite
    //
    // Note: 1) The value should not be nullptr. The texture itself should be set using
    //          the SpriteCreateInfo structure, and the meta data should be loaded using
    //          the 'LoadAnimations' method on the corresponding JSON file.
    //
    //       2) The destruction of the sprite texture is handled by the RenderWindow
    //          instance that loaded the texture.
    SDL_Texture* m_texture;

    // The current animation frame of the sprite
    //
    // Note: This variable can only be populated by calling the 'LoadAnimations' method
    //       on the JSON file containing the sprite's meta data.
    Frame m_currentFrame;

    // Hast table that maps all of the animations to a unique name
    //
    // Note: This animation set can only be populated by calling the 'LoadAnimations'
    //       method on the JSON file containing the sprite's meta data.
    std::unordered_map<std::string, Animation> m_animationSet;

    // Boolean that determines if the sprite uses world coordinates
    //
    // Note: For sprites belonging to game objects (with physics bodies), the value
    //       should be false, however, setting the field to true will have no effect.
    bool m_useScreenCoord;

    // The scroll factor used for parallax scrolling
    //
    // Note: 1) For sprite belonging to game objects, the value of the scroll factor
    //          should be 1.0f. For sprites in the background, the value should be
    //          between 0.0f and 1.0f, and for sprites in the foreground, the value
    //          should be greater than 1.0f.
    //
    //       2) Unlike the m_useScreenCoord field, setting this to a value other than
    //          1.0f for sprites belonging to game objects will call unexpected behavior.
    float m_scrollFactor;
};
