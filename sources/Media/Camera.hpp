#pragma once

#include <box2d/b2_math.h>    // b2Vec2 class

class Window;


/**
 * Class for implementing the game world camera
 */
class Camera
{
public:
    // IMPLEMENTATION
    // ==============
    
    // Default Constructor
    Camera();

    // Parent Window Constructor
    Camera(const Window& window);

    // Returns the camera transform
    b2Vec2 GetTransform() const;

    // Updates the point that is being viewed by the camera
    void Update(float x, float y);

    // Updates the point that is being viewed by the camera
    void Update(b2Vec2 point);

    // Resets the camera transform
    void Reset();

private:
    // Pointer to the parent window of the camera
    const Window* m_parent;

    // Transform applied to world coordinates to create the scrolling effect
    //
    // Note: The transform is updated by the 'Update' method (in each game
    //       loop iteration).
    b2Vec2 m_transform;
};
