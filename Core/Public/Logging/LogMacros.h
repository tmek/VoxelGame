// copyright

#pragma once
#include "LogVerbosity.h"
#include "Generated/CoreExports.h"

// Log categories


// CORE_API DECLARE_LOG_CATEGORY_EXTERN(LogHAL, Log, All);
#define DECLARE_LOG_CATEGORY_EXTERN(CategoryName, DefaultVerbosity, CompileTimeVerbosity) extern const wchar_t* ##CategoryName
#define DEFINE_LOG_CATEGORY(CategoryName) const wchar_t* ##CategoryName

CORE_API DECLARE_LOG_CATEGORY_EXTERN(LogInput, Log, All);
CORE_API DECLARE_LOG_CATEGORY_EXTERN(LogConsole, Log, All);
CORE_API DECLARE_LOG_CATEGORY_EXTERN(LogGraphics, Log, All);


// Logging
#define TE_LOG(Category, Verbosity, Format, ...) TELog(L"LogTemp", ELogVerbosity::Verbosity, Format, ##__VA_ARGS__)
#define UE_LOG(Category, Verbosity, Format, ...) TELog(L"LogTemp", ELogVerbosity::Verbosity, Format, ##__VA_ARGS__)


extern void CORE_API TELog(const wchar_t* CategoryString, ELogVerbosity::Type Verbosity, const wchar_t* Format, ...);
