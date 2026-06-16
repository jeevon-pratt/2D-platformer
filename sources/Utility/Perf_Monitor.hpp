#pragma once

union SDL_Event;
class ImGuiContext;
class Renderer;


/**
 * Class for analyzing the game performance
 */
class PerfMonitor final
{
public:
    // IMPLEMENTATION
    // ==============

    // The Constructor
    PerfMonitor(const Renderer& renderer);

    // Sets the time interval on which frame rate display updates
    void SetInterval(uint32_t intervalTime);

    // Sets the internal ImGui context as the current context
    void SetCurrentContext();

    // Calculates the current frame time and frame rate
    //
    // Notes: This method should be called once per game loop iteration.
    void CalculateFrameRate();

    // Handles user input for the monitor window
    void HandleInput(const SDL_Event& event) const;

    // Renders a debugging window with the performance information
    void Render() const;

    // Destroys the internal ImGui context and SDL backends
    ~PerfMonitor();

private:
    // INTERNAL FUNCTIONS
    // ==================

    // Note: The copy constructor and assignment have been disabled to prevent
    //       the copying of an ImGui context
    PerfMonitor(const PerfMonitor&) = delete;
    void operator=(const PerfMonitor&) = delete;

private:
    // The internal SDL renderer context responsible for displaying info
    const SDL_Renderer* m_rendererContext;

    // The internal ImGui context
    ImGuiContext* m_guiContext;

    // Boolean that determines if ImGui and the SDL backends are initialized
    bool m_backend;

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
