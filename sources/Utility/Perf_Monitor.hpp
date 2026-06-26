#pragma once

/**
 * Class for analyzing the game performance
 */
class PerfMonitor final
{
public:
    // IMPLEMENTATION
    // ==============

    // The Constructor
    PerfMonitor();

    // Sets the time interval on which frame rate display updates
    void SetInterval(uint32_t intervalTime);

    // Calculates the current frame time and frame rate
    //
    // Notes: This method should be called once per game loop iteration.
    void CalculateFrameRate();

    // Returns the frame rate
    uint32_t GetFrameRate() const;

    // Returns the frame time
    uint32_t GetFrameTime() const;

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

    // The game frame rate.
    //
    // Note: The frame rate is calculated in the 'CalculateFrameRate' method.
    uint32_t m_frameRate;

    // The game frame time.
    //
    // Note: The frame time is calculated in the 'CalculateFrameRate' method.
    uint32_t m_frameTime;
};
