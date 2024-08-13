#pragma once

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG
};

#define LOG_CATEGORY_GENERAL "General"
#define LOG_CATEGORY_GRAPHICS "Graphics"
#define LOG_CATEGORY_INPUT "Input"

#define VG_LOG(Category, Level, Format, ...) VGLog(Category, Level, Format, ##__VA_ARGS__)

void VGLog(const char* category, LogLevel level, const char* format, ...);