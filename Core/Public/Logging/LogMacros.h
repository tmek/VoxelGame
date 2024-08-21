#pragma once
// #include "CoreTypes.h"
#include "Generated/CoreExports.h"
#include "LogVerbosity.h"

// Log categories

#define LogInit "LogInit"
#define LogConsole "LogConsole"
#define LogGeneral "LogGeneral"
#define LogGraphics "LogGraphics"
#define LogInput "LogInput"
#define LogTemp "LogTemp"


// Logging
#define TE_LOG(Category, Verbosity, Format, ...) TELog(Category, ELogVerbosity::Verbosity, Format, ##__VA_ARGS__)


void CORE_API TELog(const char* CategoryString, ELogVerbosity::Type Verbosity, const char* Format, ...);
