#include <cstdarg>
#include <cstdio>
#include <mutex>
#include <fstream>
#include "Logging/LogMacros.h"

#include "LogFileManager.h"

// Make thread safe when using stdout/stderr
std::mutex LogMutex;


void TELog(const wchar_t* CategoryString, ELogVerbosity::Type Verbosity, const wchar_t* Format, ...)
{
    wchar_t LogMessage[1024];

    // Create the formatted message
    va_list args;
    va_start(args, Format);
    (void)vswprintf_s(LogMessage, Format, args);
    va_end(args);

    // Log level
    const wchar_t* VerbosityString = L"";
    switch (Verbosity)
    {
    case ELogVerbosity::Log: VerbosityString = L"Log"; break;
    case ELogVerbosity::Warning: VerbosityString = L"Warning"; break;
    case ELogVerbosity::Error: VerbosityString = L"Error"; break;
    case ELogVerbosity::Display: VerbosityString = L"Display"; break;
    case ELogVerbosity::Fatal: VerbosityString = L"Fatal"; break;
    case ELogVerbosity::Verbose: VerbosityString = L"Verbose"; break;
    case ELogVerbosity::VeryVerbose: VerbosityString = L"VeryVerbose"; break;
    default: VerbosityString = L"Log";
    }

    {
        std::lock_guard<std::mutex> lock(LogMutex);

        // Output to console
        if (Verbosity == ELogVerbosity::Error)
        {
            (void)fwprintf(stderr, L"%s: %s: %s\n", CategoryString, VerbosityString, LogMessage);
            (void)fflush(stderr);
        }
        else
        {
            (void)fwprintf(stdout, L"%s: %s: %s\n", CategoryString, VerbosityString, LogMessage);
            (void)fflush(stdout);
        }

        GLogManager.WriteToLog(LogMessage);
    }
}
