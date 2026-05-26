#pragma once

#include <SDL2/SDL_Log.h>       // SDL logging functions

#include <string_view>          // std::string_view


// ************
// LOG PRIORITY
// ************

enum LOG_PRIORITY
{
    LOG_PRIORITY_VERBOSE = 1,
    LOG_PRIORITY_DEBUG,
    LOG_PRIORITY_INFO,
    LOG_PRIORITY_WARN,
    LOG_PRIORITY_ERROR,
    LOG_PRIORITY_CRITICAL,
    NUM_LOG_PRIORITIES
};



// *****************
// LOGGING FUNCTIONS
// *****************

// Clears and opens the specified log file and sets up the custom logging system
//
//  Note: 1) This function returns true on success and false on failure.
//
//        2) This function should be called before attempting to use the SDL logging system
//           or the custom logging macro functions. Otherwise, all debugging message will
//           be output to the console instead of a log file.
bool InitLog(std::string_view filepath, LOG_PRIORITY priority);


// Returns a boolean indicating if the logging system is initialized
//
// Note: This function is used in the logging macros to prevent SDl from logging to the
//       console if the logging system is unitialized.
bool LogFileOpen();


//  Custom log function that outputs to a .txt log file
//
//  Note: 1) A newline is printed at the end of each message.
//
//        2) This function should not be used directly. The function pointer is passed into
//           the 'SDL_LogSetOutputFunction' function to set up the custom logging system.
//
//        3) According to the SDL documentation, the function pointer must have this
//           specific signature to be compatible.
void LogToFile(void* userdata, int category, SDL_LogPriority priority, const char* message);


// Closes the log file
//
// Note: This function has no effect if called when the loggings system is uninitialized.
void CloseLogFile();



// ***********************
// LOGGING MACRO FUNCTIONS
// ***********************

/**
 *  Debug macro functions for logging that are disabled for the release configuration
 */
#ifdef DEBUG

    // Logs messages with priority SDL_LOG_PRIORITY_VERBOSE
    #define GAME_2D_LOG_VERBOSE(fmt, ...) \
        if ( LogFileOpen() ) \
            SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, fmt, __VA_ARGS__)


    // Logs messages with priority SDL_LOG_PRIORITY_DEBUG
    #define GAME_2D_LOG_DEBUG(fmt, ...) \
        if ( LogFileOpen() ) \
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, fmt, __VA_ARGS__)


    // Logs messages with priority SDL_LOG_PRIORITY_INFO
    #define GAME_2D_LOG_INFO(fmt, ...) \
        if ( LogFileOpen() ) \
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, fmt, __VA_ARGS__)


    // Logs messages with priority SDL_LOG_PRIORITY_WARN
    #define GAME_2D_LOG_WARN(fmt, ...) \
        if ( LogFileOpen() ) \
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, fmt, __VA_ARGS__)


    // Logs messages with priority SDL_LOG_PRIORITY_ERROR
    #define GAME_2D_LOG_ERROR(fmt, ...) \
        if ( LogFileOpen() ) \
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, fmt, __VA_ARGS__)


    // Logs messages with priority SDL_LOG_PRIORITY_CRITICAL
    #define GAME_2D_LOG_CRITICAL(fmt, ...) \
        if ( LogFileOpen() ) \
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, fmt, __VA_ARGS__)

#else

    #define GAME_2D_LOG_FALLTHROUGH do {} while (false)

    #define GAME_2D_LOG_VERBOSE(fmt, ...)   GAME_2D_LOG_FALLTHROUGH
    #define GAME_2D_LOG_INFO(fmt, ...)      GAME_2D_LOG_FALLTHROUGH
    #define GAME_2D_LOG_DEBUG(fmt, ...)     GAME_2D_LOG_FALLTHROUGH
    #define GAME_2D_LOG_WARN(fmt, ...)      GAME_2D_LOG_FALLTHROUGH
    #define GAME_2D_LOG_ERROR(fmt, ...)     GAME_2D_LOG_FALLTHROUGH
    #define GAME_2D_LOG_CRITICAL(fmt, ...)  GAME_2D_LOG_FALLTHROUGH

#endif

