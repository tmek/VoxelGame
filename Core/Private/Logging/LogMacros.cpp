#include "Logging/LogMacros.h"
#include <cstdio>
#include <cstdarg>
#include <mutex>

// make thread safe when using stdout/stderr
std::mutex LogMutex;


void TELog(const char* CategoryString, ELogVerbosity::Type Verbosity, const char* Format, ...)
{
    char LogMessage[1024];

    // Create the formatted message
    va_list args;
    va_start(args, Format);
    (void)vsprintf_s(LogMessage, Format, args);
    va_end(args);

    // log level
    auto VerbosityString = "";
    switch (Verbosity)
    {
    case ELogVerbosity::Log: VerbosityString = "Log";
        break;
    case ELogVerbosity::Warning: VerbosityString = "Warning";
        break;
    case ELogVerbosity::Error: VerbosityString = "Error";
        break;
    case ELogVerbosity::Display: VerbosityString = "Display";
        break;
    case ELogVerbosity::Fatal: VerbosityString = "Fatal";
        break;
    case ELogVerbosity::Verbose: VerbosityString = "Verbose";
        break;
    case ELogVerbosity::VeryVerbose: VerbosityString = "VeryVerbose";
        break;
    default:
        VerbosityString = "Log";
    }


    {
        std::lock_guard<std::mutex> lock(LogMutex);
        
        // output to console
        if (Verbosity == ELogVerbosity::Error)
        {
            (void)fprintf(stderr, "%s: %s: %s\n", CategoryString, VerbosityString, LogMessage);
            (void)fflush(stderr);
        }
        else
        {
            (void)fprintf(stdout, "%s: %s: %s\n", CategoryString, VerbosityString, LogMessage);
            (void)fflush(stdout);
        }
    }
}
