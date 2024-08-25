// Copyright 

//#undef _UNICODE

#include <tchar.h>

#ifdef _UNICODE
#define TEXT(quote) _T(quote)
#else
#define TEXT(quote) _TEXT(quote)
#endif

#include <iostream>



void TCHARExperiments(char** ArgumentsArray)
{
    // disable stdout buffering
    setvbuf(stdout, nullptr, _IONBF, 0);

    
    int TCHAR_SIZE = sizeof(TCHAR);
    std::wcout << "TCHAR size: " << TCHAR_SIZE << '\n';
    
    // Convert first argument to TCHAR
    wchar_t FirstArgumentW[256];
    size_t NumConverted = 0;
    if(mbstowcs_s(&NumConverted, FirstArgumentW, ArgumentsArray[1], 256) != 0)
    {
        std::wcerr << "Failed to convert first argument to TCHAR!" << '\n';
        return;
    }
        
    TCHAR* FirstArgument = FirstArgumentW;
    std::wcout << "First argument (TCHAR*): " << FirstArgument << '\n';
    
    uint32_t MyNumber = 42;

    TCHAR MyCharacter = 'A';
    constexpr TCHAR MyString[] = TEXT("TCHAR!");
    constexpr int MyStringSize = sizeof(MyString);

    // write to a string buffer
    TCHAR tcscpyBuffer[256];
    _tcscpy_s(tcscpyBuffer, TEXT("TCHAR copied to tcscpyBuffer"));


    // output
    std::wcout << "Hello, World!" << '\n';

    std::wcout << "First argument (char*): " << FirstArgument << '\n';
    std::wcout << "My number: " << MyNumber << '\n';
    std::wcout << "My character (TCHAR): " << MyCharacter << '\n';
    std::wcout << "My string (TCHAR*): " << MyString << '\n';

    std::wcout << "tcscpyBuffer (TCHAR[]): " << tcscpyBuffer << '\n';

    // format to a string buffer
    TCHAR tprintfBuffer[256] = {};
    (void)_stprintf_s(tprintfBuffer, TEXT("The magic number is %d!"), MyNumber);
    std::wcout << "printfBuffer (TCHAR[]): " << tprintfBuffer << '\n';
}
int main(int ArgumentsCount, char* ArgumentsArray[])
{
    for(int i=0; i<ArgumentsCount; ++i)
    {
         
        std::cout << "Argument " << i << ": " << ArgumentsArray[i] << '\n';
    }
    
    TCHARExperiments(ArgumentsArray);

    return 0;
}
