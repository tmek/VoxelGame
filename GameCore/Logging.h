#pragma once

enum LogVerbosity {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG
};

#define LogCategoryGeneral "General"
#define LogCategoryGraphics "Graphics"
#define LogCategoryInput "Input"

#define VG_LOG(Category, Level, Format, ...) VGLog(Category, Level, Format, ##__VA_ARGS__)

void VGLog(const char* category, LogVerbosity level, const char* format, ...);