#pragma once

// Use this file to include the minimal set of headers required for Windows platform.
// Include this in .h files and include PlatformWindows.h in their .cpp files.

// You must include PlatformWindows.h in the .cpp file above the #include that then includes PlatformWindowsMinimal.h
// (windows.h must precede <windef.h> in the translation unit)

// lean and mean removes some of the less used APIs
#define WIN32_LEAN_AND_MEAN
#include <windef.h>