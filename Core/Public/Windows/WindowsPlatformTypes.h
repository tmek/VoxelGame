#pragma once

// A wide character. In-memory only. ?-bit fixed-width representation of the platform's natural wide character set. Could be different sizes on different platforms.
using WIDECHAR = wchar_t;


// A switchable character. In-memory only. Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.
using TCHAR = WIDECHAR;


// integers
using int8 = signed char;
using int16 = signed short;
using int32 = signed int;
using int64 = signed long long;

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;
