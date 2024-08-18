// Copyright Voxel Games, Inc. All Rights Reserved.

#include "LaunchEngineLoop.h"

FEngineLoop GEngineLoop;

bool GIsRequestingExit = false;

int32 GuardedMain(const TCHAR* CmdLine)
{
    VG_LOG(LogCategoryGeneral, LOG_INFO, "GuardedMain(\"%ls\")", CmdLine);

    VG_LOG(LogCategoryGeneral, LOG_INFO, "Initializing Engine Loop...");
    GEngineLoop.Init();

    VG_LOG(LogCategoryGeneral, LOG_INFO, "Entering Engine Loop...");
    while (!GIsRequestingExit)
    {
        GEngineLoop.Tick();
    }

    VG_LOG(LogCategoryGeneral, LOG_INFO, "Exiting Engine Loop...");
    GEngineLoop.Exit();

    VG_LOG(LogCategoryGeneral, LOG_INFO, "Exiting GuardedMain...");
    return 0;
}
