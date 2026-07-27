#pragma once

#include <SDL3/SDL_rect.h>    // SDL_FRect struct

#include <vector>             // std::vector


/**
 * Structure for storing frame meta data
 */
struct Frame
{
    SDL_FRect srcrect;   // The source SDL_Rect structure of the frame image
    uint8_t   duration;  // The duration of the frame's appearance in milliseconds


    // Default Constructor
    Frame() = default;

    // Member Constructor
    Frame(const SDL_FRect& rect, uint8_t value):
        srcrect {rect},  duration (value)  {}
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

    // Adds a frame structure to the vector of frame meta data
    void AddFrame(Frame frame);

    // Returns a boolean indicating whether the elasped time exceeds the duration
    // of the current animation frame
    bool FrameTimeExceeded() const;

    // Returns a boolean that determines if an animation cycle has been completed
    bool CycleCompleted() const; 

    // Returns the current animation frame
    Frame GetCurrentFrame() const;

    // Resets the animation cycle
    void Reset();

    // Plays the animation cycle
    void Play();

private:
    // Holds the start time for the internal animation timer
    //
    // Note: This value is modified in the 'StepTime' method.
    uint32_t m_currentTime;

    // The elasped time of the animation cycle
    //
    // Note: The frame duration for animation must not be 0. Otherwise, the
    //       animation will not be displayed properly.
    uint32_t m_elapsedTime;

    // The meta data for each animation frame
    //
    // Note: This vector can be populated by calling the 'AddFrame' method
    //       when loading sprite meta data.
    std::vector<Frame> m_frames;

    // The index to the current animation frame
    uint8_t m_index;
};
