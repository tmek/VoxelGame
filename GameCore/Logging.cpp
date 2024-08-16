#include <cstdio>
#include <cstdarg>
#include "Logging.h"

void VGLog(const char* category, LogVerbosity level, const char* format, ...)
{
    char lineBuffer[1024];

    // Create the formatted message
    va_list args;
    va_start(args, format);
    (void)vsprintf_s(lineBuffer, format, args);
    va_end(args);

    // log level
    const char* levelStr = "";
    switch (level) {
    case LOG_INFO: levelStr = "INFO"; break;
    case LOG_WARNING: levelStr = "WARNING"; break;
    case LOG_ERROR: levelStr = "ERROR"; break;
    case LOG_DEBUG: levelStr = "DEBUG"; break;
    }

    // output to console
    printf("[%s] [%s]: %s\n", levelStr, category, lineBuffer);

    fflush(stdout);  // Ensure immediate output
}
