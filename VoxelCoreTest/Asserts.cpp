#include "Asserts.h"

#include <cstdarg>
#include <cstdio>
#include <intrin.h>

#include "Voxel/VoxelDefines.h"

void BeginTestGroup(const char* str)
{ 
    printf("Test group: %s\n", str);
}

void AssertAreEqual(int a, int b, const char* format, ...)
{
    char buffer[1024];

    // Create the formatted message
    va_list args;
    va_start(args, format);
    (void)vsprintf_s(buffer, format, args);
    va_end(args);    
    if(a != b)
    {
        printf("    AssertAreEqual failed: %d != %d\n", a, b);
        if(format != nullptr)
        {
            printf("Message: %s\n", buffer);
        }
        PLATFORM_BREAK();
        return;
    }

    printf("    Passed: %s\n", buffer);
}
