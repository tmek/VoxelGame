#include <cstdio>
#include <cstdarg>
#include "Logging.h"

void VGLog(const char* category, LogLevel level, const char* format, ...)
{
    char buffer[1024];

    // Create the formatted message
    va_list args;
    va_start(args, format);
    (void)vsprintf_s(buffer, format, args);
    va_end(args);

    // Output the log message
    const char* levelStr = "";
    switch (level) {
    case LOG_INFO: levelStr = "INFO"; break;
    case LOG_WARNING: levelStr = "WARNING"; break;
    case LOG_ERROR: levelStr = "ERROR"; break;
    case LOG_DEBUG: levelStr = "DEBUG"; break;
    }

    printf("[%s] [%s]: %s\n", levelStr, category, buffer);
    fflush(stdout);  // Ensure immediate output
}
