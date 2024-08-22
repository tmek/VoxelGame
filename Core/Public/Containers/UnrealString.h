// copyright

#pragma once

#include <string>
#include <tchar.h>


class FString : public std::wstring
{
public:
    // * unary operator returns a const TCHAR* (wchar_t*)
    const TCHAR* operator*() const
    {
        return c_str();
    }

};


// Helper function to convert std::string to TCHAR* (wchar_t*)
inline TCHAR* FStringToTCHAR(const std::wstring& s)
{
    // Allocate memory for TCHAR*
    TCHAR* tcharStr = new TCHAR[s.size() + 1];
    
    // Copy the wide string to the TCHAR array
    std::copy(s.begin(), s.end(), tcharStr);

    // Null-terminate the TCHAR array
    tcharStr[s.size()] = 0; 
    
    return tcharStr;
}

// Helper function to convert TCHAR* (wchar_t*) to std::string
inline std::wstring TCHARToFString(TCHAR* s)
{
    // Convert TCHAR* (wide character) to std::wstring
    return std::wstring(s);
}


// Overloaded operator to automatically convert TCHAR* to std::string
inline std::wstring operator+(const std::wstring& lhs, TCHAR* rhs)
{
    return lhs + TCHARToFString(rhs);
}

// Overloaded operator to automatically convert std::string to TCHAR*
inline TCHAR* operator+(TCHAR* lhs, const std::wstring& rhs)
{
    return FStringToTCHAR(rhs);
}


using FName = FString;