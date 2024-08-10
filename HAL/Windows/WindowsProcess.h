#pragma once

#include "Core/Core.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>

struct WindowsProcess
{
    static void Sleep(int32 Milliseconds)
    {
        ::Sleep(Milliseconds);
    }

    static void ShowConsole()
    {
        if (!AllocConsole()) {
            fprintf(stderr, "Error allocating console\n");
            return;
        }

        HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
        if (handle_out == INVALID_HANDLE_VALUE) {
            fprintf(stderr, "Error getting standard output handle\n");
            return;
        }

        int hCrt = _open_osfhandle((intptr_t)handle_out, _O_TEXT);
        if (hCrt == -1) {
            fprintf(stderr, "Error opening OS handle\n");
            return;
        }

        FILE* hf_out = _fdopen(hCrt, "w");
        if (!hf_out) {
            fprintf(stderr, "Error associating file stream\n");
            return;
        }

        setvbuf(hf_out, NULL, _IONBF, 0);
        *stdout = *hf_out;

        HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
        if (handle_in == INVALID_HANDLE_VALUE) {
            fprintf(stderr, "Error getting standard input handle\n");
            return;
        }

        hCrt = _open_osfhandle((intptr_t)handle_in, _O_TEXT);
        if (hCrt == -1) {
            fprintf(stderr, "Error opening OS handle\n");
            return;
        }

        FILE* hf_in = _fdopen(hCrt, "r");
        if (!hf_in) {
            fprintf(stderr, "Error associating file stream\n");
            return;
        }

        setvbuf(hf_in, NULL, _IONBF, 0);
        *stdin = *hf_in;

        // use the console just like a normal one - printf(), getchar(), ...
    }
    
    // static void ShowConsoleOld()
    // {
    //     AllocConsole();
    //
    //     
    //     HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    //     int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
    //     FILE* hf_out = _fdopen(hCrt, "w");
    //     setvbuf(hf_out, NULL, _IONBF, 1);
    //     *stdout = *hf_out;
    //
    //     HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    //     hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
    //     FILE* hf_in = _fdopen(hCrt, "r");
    //     setvbuf(hf_in, NULL, _IONBF, 128);
    //     *stdin = *hf_in;
    //
    //     // use the console just like a normal one - printf(), getchar(), ...
    // }

};

typedef WindowsProcess PlatformProcess;
