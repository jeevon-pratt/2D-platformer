#pragma once

#include <SDL2/SDL_rect.h>  // SDL_Rect struct

#include <vector>           // std::vector


/**
 * Structure for storing frame meta data
 */
struct Frame
{
    SDL_Rect srcrect;   // The source SDL_Rect structure of the frame image
    uint8_t  duration;  // The duration of the frame's appearance in milliseconds
};


/**
 * Class for implementing sprite animations
 */
class Animation final
{
public:
    // IMPLEMENTATION
    // ==============

    // Default Constructor
    Animation();

    // Returns the current animation frame
    const Frame& GetCurrentFrame() const;

    // Adds a frame structure to the vector of frame meta data
    void AddFrame(const Frame& frame);

    // Progresses the animation cycle
    void NextFrame();

    // Increments the animation cycle's elapsed time
    void StepTime();

    // A function used by the sprite class to determine whether the elasped time
    // exceeds the set duration of the current animation frame
    bool FrameTimeExceeded() const;

    // Returns a boolean that determines if an animation cycle has been completed
    bool CycleComplete() const;

private:
    // Boolean that determines if an animation cycle has been completed
    //
    // Note: This variable is modified by the 'StepCycle' method.
    bool m_cycleComplete;

    // Holds the start time for the internal animation timer
    //
    // Note: This value is modified in the 'StepTime' method.
    uint32_t m_currentTime;

    // The elasped time of the animation cycle
    //
    // Note: The frame duration for animation must not be 0. Otherwise, the
    //       animation will not be displayed properly.
    uint8_t m_elapsedTime;

    // The meta data for each animation frame
    //
    // Note: This vector can be populated by calling the 'AddFrame' method
    //       when loading sprite meta data.
    std::vector<Frame> m_frames;

    // The std::vector index to the current animation frame
    //
    // Note: When a cycle is completed, the current index loops back to 0.
    uint16_t m_currentIndex;
};
