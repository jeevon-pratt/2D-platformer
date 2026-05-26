#include <box2d/b2_body.h>          // b2Vec2 class

#include <cmath>                    // abs, pow, and sqrt functions
#include <iostream>                 // std::ostream
#include <limits>                   // std::numeric_limits<float>::epsilon constant

#include "Entity/Game_Object.hpp"   // GameObject class
#include "Media/Renderer.hpp"       // Renderer::PIXELS_PER_METER constant
#include "Media/Sprite.hpp"         // Sprite class


// *****************************
// 'b2Vec2' NON-MEMBER FUNCTIONS
// *****************************

b2Vec2 operator*(b2Vec2 vec, float scalar)
{
    return b2Vec2( (vec.x * scalar), (vec.y * scalar) );
}



b2Vec2 operator/(b2Vec2 vec, float scalar)
{
    return (vec * (1.0f / scalar));
}



std::ostream& operator<<(std::ostream &out, b2Vec2 vec)
{
    return out << '(' << vec.x << ", " << vec.y << ')';
}



// ********************
// CONVERSION FUNCTIONS
// ********************

float ConvertToPixels(float x)
{
    return (x * Renderer::PIXELS_PER_METER);
}



float ConvertToMeters(float x)
{
    return (x / Renderer::PIXELS_PER_METER);
}



b2Vec2 ConvertToScreenCoord(const Sprite& sprite, b2Vec2 worldPos, b2Vec2 camTransform)
{
    b2Vec2 screenPos;

    // Convert the world coordinates to screen coordinates
    screenPos.x = ConvertToPixels(worldPos.x);
    screenPos.y = ConvertToPixels(worldPos.y);

    // Reposition coordinate to top-left corner of texture
    screenPos.x -= (sprite.GetFrameWidth() / 2.0f);
    screenPos.y += (sprite.GetFrameHeight() / 2.0f);

    // Modify the coordinates to use the game camera
    screenPos.x -= (camTransform.x * sprite.GetScrollFactor());
    screenPos.y -= (camTransform.y * sprite.GetScrollFactor());

    // Invert y-axis
    screenPos.y *= -1;

    return screenPos;
}



// *************
// MISCELLANEOUS
// *************

bool ApproxEq(float num1, float num2)
{
    return abs(num1 - num2) <= ( std::max(num1, num2) * std::numeric_limits<float>::epsilon() );
}



float GetDistance(b2Vec2 p1, b2Vec2 p2)
{
    float deltaX = p2.x - p1.x;
    float deltaY = p2.y - p1.y;
    float squareDistance = pow(deltaX, 2.0f) + pow(deltaY, 2.0f);

    return sqrt(squareDistance);
}



b2Vec2 Midpoint(b2Vec2 p1, b2Vec2 p2)
{
    b2Vec2 midpoint;
    midpoint.x = (p1.x + p2.x) / 2.0f;
    midpoint.y = (p1.y + p2.y) / 2.0f;

    return midpoint;
}
