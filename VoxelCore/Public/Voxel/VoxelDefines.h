#pragma once

#include "Generated/Exports.h"

//#include "intrin.h" // __nop // todo: remove this dependency at least in such a core include file
// Q: Why is there a __nop() before __debugbreak()?
// A: VS' debug engine has a bug where it will silently swallow explicit
// breakpoint interrupts when single-step debugging either line-by-line or
// over call instructions. This can hide legitimate reasons to trap. Asserts
// for example, which can appear as if the did not fire, leaving a programmer
// unknowingly debugging an undefined process.
//#define PLATFORM_BREAK() (__nop(), __debugbreak())
#define PLATFORM_BREAK() (__debugbreak())

#ifndef check
#define check(expr) if(!(expr)) PLATFORM_BREAK()
#endif
