#pragma once

#include <box2d/b2_math.h>  // b2Vec2 class


/**
 * Class for implementing the game world camera
 */
class Camera
{
public:
    // IMPLEMENTATION
    // ==============
    
    // The default constructor
    Camera();

    // Returns the camera transform
    b2Vec2 GetTransform() const;

    // Updates the point that is being viewed by the camera
    void Update(b2Vec2 focusPoint, uint16_t viewWidth, uint16_t viewHeight);

    // Resets the camera transform
    void Reset();

private:
    // Transform applied to world coordinates to create the scrolling effect
    //
    // Note: The transform is updated by the 'Update' method (in each game
    //       loop iteration).
    b2Vec2 m_transform;
};
