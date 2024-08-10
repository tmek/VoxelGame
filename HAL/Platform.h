#pragma once

// A wide character. In-memory only. ?-bit fixed-width representation of the platform's natural wide character set. Could be different sizes on different platforms.
typedef wchar_t WIDECHAR;

// A switchable character. In-memory only. Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.
typedef WIDECHAR TCHAR;

typedef signed char         int8;
typedef signed int          int32;
typedef signed long long    int64;

typedef unsigned char       uint8;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
