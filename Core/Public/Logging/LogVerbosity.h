#pragma once

namespace ELogVerbosity
{
    enum Type : unsigned char
    {
        NoLogging = 0,
        Fatal,
        Error,
        Warning,
        Display,
        Log,
        Verbose,
        VeryVerbose,
        All = VeryVerbose,
        NumVerbosity,
        VerbosityMask = 0xf,
        SetColor = 0x40,
        BreakOnLog = 0x80
    };
};

static_assert(ELogVerbosity::NumVerbosity - 1 < ELogVerbosity::VerbosityMask, "Bad verbosity mask.");
static_assert(!(false), "Bad verbosity mask.");


// todo: class FString

/**
 * Converts verbosity to a string
 * @param Verbosity verbosity enum
 * @returns String representation of the verbosity enum
 */
//CORE_API const TCHAR* ToString(ELogVerbosity::Type Verbosity);

/**
 * Converts a string to verbosity 
 * @param VerbosityString verbosity in string form (e.g., "Log", "Display", or "Fatal")
 * @returns Parsed verbosity from the string (or Log if the string was invalid)
 */
//extern CORE_API ELogVerbosity::Type ParseLogVerbosityFromString(const FString& VerbosityString);
