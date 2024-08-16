// Copyright Voxel Games, Inc. All Rights Reserved.

#include "LaunchEngineLoop.h"

FEngineLoop GEngineLoop;

bool GIsRequestingExit = false;

int32 GuardedMain(const TCHAR* CmdLine)
{
    VG_LOG(LogCategoryGeneral, LOG_INFO, "GuardedMain(\"%ls\")", CmdLine);
    
    GEngineLoop.Init();

    while (!GIsRequestingExit)
    {
        GEngineLoop.Tick();
    }

    GEngineLoop.Exit();
    return 0;
}
