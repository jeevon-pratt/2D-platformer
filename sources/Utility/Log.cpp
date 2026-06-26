#include <SDL3/SDL_log.h>       // SDL_SetLogOutputFunction and SDL_SetLogPriority functions
#include <SDL3/SDL_timer.h>     // SDL_GetTicks function

#include <fstream>              // std::ofstream
#include <iostream>             // std::cerr
#include <string>               // std::string

#include "Log.hpp"              // Logging functions declarations


// *****************
// LOGGING FUNCTIONS
// *****************

// Static file stream object which outputs to the log file
static std::ofstream s_logFile;



bool InitLog(const std::string& filepath, LogPriority priority)
{
    SDL_SetLogOutputFunction(LogToFile, nullptr);
    SDL_SetLogPriority( SDL_LOG_CATEGORY_APPLICATION, static_cast<SDL_LogPriority>(priority) );


    // To create the log file if it does not exist and clear the contents if it does
    s_logFile.open(filepath, std::ios::out | std::ios::trunc);
    s_logFile.close();

    s_logFile.open(filepath, std::ios::app);


    return s_logFile.is_open();
}



bool LogFileOpen()
{
    return s_logFile.is_open();
}



void LogToFile(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
    s_logFile << SDL_GetTicks() << " ms\t";


    switch (priority)
    {
    case SDL_LOG_PRIORITY_INVALID:
        s_logFile << "INVALID: ";
        break;
    case SDL_LOG_PRIORITY_TRACE:
        s_logFile << "TRACE: ";
        break;
    case SDL_LOG_PRIORITY_VERBOSE:
        s_logFile << "VERBOSE: ";
        break;
    case SDL_LOG_PRIORITY_DEBUG:
        s_logFile << "DEBUG: ";
        break;
    case SDL_LOG_PRIORITY_INFO:
        s_logFile << "INFO: ";
        break;
    case SDL_LOG_PRIORITY_WARN:
        s_logFile << "WARNING: ";
        break;
    case SDL_LOG_PRIORITY_ERROR:
        s_logFile << "ERROR: ";
        break;
    case SDL_LOG_PRIORITY_CRITICAL:
        s_logFile << "CRITICAL: ";
        break;
    default:
        s_logFile << "UNKNOWN: ";
        break;
    }


    s_logFile << message << '\n';
}



void CloseLogFile()
{
    s_logFile.close();
}
