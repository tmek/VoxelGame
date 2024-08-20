#pragma once

// A 1-byte character. In-memory only. 8-bit fixed-width representation of the platform's natural ANSI character set.
typedef char ANSICHAR;

// A wide character. In-memory only. ?-bit fixed-width representation of the platform's natural wide character set. Could be different sizes on different platforms.
typedef wchar_t WIDECHAR;

// A switchable character. In-memory only. Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.
#ifdef _UNICODE
    typedef WIDECHAR TCHAR;
#else
    typedef ANSICHAR TCHAR;
#endif

// #ifndef TEXT
//     #ifdef _UNICODE
//         #define TEXT(x) L##x
//     #else
//         #define TEXT(x) x
//     #endif
// #endif


// Integer types
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
