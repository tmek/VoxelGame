#pragma once
#include "Generated/Exports.h"
#include <intrin.h> // for __nop()

#define EngineName "TankEngine"
#define ModuleName "TankEngine.Core"

// Q: Why is there a __nop() before __debugbreak()?
// A: VS' debug engine has a bug where it will silently swallow explicit
// breakpoint interrupts when single-step debugging either line-by-line or
// over call instructions. This can hide legitimate reasons to trap. Asserts
// for example, which can appear as if the did not fire, leaving a programmer
// unknowingly debugging an undefined process.
#define PLATFORM_BREAK() (__nop(), __debugbreak())

// debug print one time per application instance
#ifndef STUBBED
#define STUBBED(x)	\
do																								\
{																								\
static bool AlreadySeenThisStubbedSection = false;											\
if (!AlreadySeenThisStubbedSection)															\
{																							\
AlreadySeenThisStubbedSection = true;													\
fprintf(stderr, "STUBBED: %s at %s:%d (%s)\n", x, __FILE__, __LINE__, __FUNCTION__);	\
}																							\
} while (0)
#endif

#ifndef check
#define check(expr) if(!(expr)) PLATFORM_BREAK()
#endif
