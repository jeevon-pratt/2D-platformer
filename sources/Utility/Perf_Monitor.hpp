#pragma once

#include "Media/Text.hpp"       // Text class

struct _TTF_Font;               // typedef struct _TTF_Font TTF_Font
struct SDL_Color;


/**
 * Class for analyzing the game performance
 */
class PerfMonitor final
{
public:
    // IMPLEMENTATION
    // ==============

    // The Default Constructor
    PerfMonitor();

    // Sets the time interval on which frame rate display updates
    void SetInterval(uint32_t intervalTime);

    // Sets the font of the frame time and frame rate text
    void SetTextFont(_TTF_Font* font);

    // Sets the color of the frame time and frame rate text
    void SetTextColor(SDL_Color color);

    // Calculates the current frame time and frame rate
    //
    // Notes: This method should be called once per game loop iteration.
    void CalculateFrameRate();

    // Returns the frame time text object for displaying to the screen
    //
    // Note: The 'SetTextFont' and "SetTextColor" methods should be called before
    //       rendering the text.
    const Text& GetFrameTimeText() const;

    // Returns the frame rate text object for displaying to the screen
    //
    // Note: The 'SetTextFont' and "SetTextColor" methods should be called before
    //       rendering the text.
    const Text& GetFrameRateText() const;

private:
    // This variable holds the start time for the internal timer.
    //
    // Note: This value is set in the 'CalculateFrameRate' method.
    uint32_t m_currentTime;

    // This variable holds the elapsed time for the internal timer.
    //
    // Note: This value is reset in the 'CalculateFrameRate' method and the
    //       'DisplayFrameRate' method.
    uint32_t m_elapsedTime;

    // This variable is the time interval on which the frame rate display updates.
    //
    // Note: If the 'SetInterval' method is not called, the default value is 1000
    //       milliseconds (1 second).
    uint32_t m_intervalTime;

    // This text object holds the game frame rate.
    //
    // Note: The frame rate value is calculated in the 'CalculateFrameRate' method.
    Text m_frameRateText;

    // This text object holds the game frame time.
    //
    // Note: The frame time value is calculated in the 'CalculateFrameRate' method.
    Text m_frameTimeText;
};
