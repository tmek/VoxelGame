// // Copyright (c) 2024 Thomas. All rights reserved.

#pragma once

#include <fstream>
#include <iomanip>
#include <mutex>
#include <iostream>
#include <sstream>

// Log file management class
class LogFileManager
{
public:
    LogFileManager(const wchar_t* BaseFileName)
    {
        // Get the current time
        std::time_t now = std::time(nullptr);
        std::tm localTime;
        localtime_s(&localTime, &now); // Thread-safe function for local time conversion

        // Create a timestamp string (YYYY-MM-DD_HH-MM-SS)
        std::wstringstream timeStream;
        timeStream << (localTime.tm_year + 1900) << L"-"
                   << std::setw(2) << std::setfill(L'0') << (localTime.tm_mon + 1) << L"-"
                   << std::setw(2) << std::setfill(L'0') << localTime.tm_mday << L"_"
                   << std::setw(2) << std::setfill(L'0') << localTime.tm_hour << L"-"
                   << std::setw(2) << std::setfill(L'0') << localTime.tm_min << L"-"
                   << std::setw(2) << std::setfill(L'0') << localTime.tm_sec;

        // Combine the timestamp with the base file name
        std::wstringstream fileNameStream;
        fileNameStream << timeStream.str() << L"_" << BaseFileName;

        // Open the file in append mode
        LogFile.open(fileNameStream.str(), std::ios::app);
        if (!LogFile.is_open())
        {
            std::wcerr << L"Error: Could not open log file " << fileNameStream.str() << '\n';
        }
        else
        {
            std::wcout << L"Log file created: " << fileNameStream.str() << std::endl;
        }
    }
    
    ~LogFileManager()
    {
        // Close the file when the application exits
        if (LogFile.is_open())
        {
            LogFile.close();
        }
    }

    // Method to write to the log file
    void WriteToLog(const wchar_t* Message)
    {
        std::lock_guard<std::mutex> lock(LogMutex);
        if (LogFile.is_open())
        {
            LogFile << Message << std::endl;
        }
    }


private:
    static std::wofstream LogFile;  // Global log file stream
    static std::mutex LogMutex;  // Mutex for thread-safe logging
};

extern LogFileManager GLogManager;


