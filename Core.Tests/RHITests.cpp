#include "gtest/gtest.h"

/*
DynamicRHI* GDynamicRHI;


// void EngineLoop::PreInitPreStartupScreen()
// {
//     InitRHI();
// }

void InitRHI()
{
    if (!GDynamicRHI)
    {
        GDynamicRHI = PlatformCreateDynamicRHI();
        GDynamicRHI->Init();
    }
}


DynamicRHI* PlatformCreateDynamicRHI()
{
    DynamicRHI* DynamicRHI = nullptr;

    ERHIFeatureLevel::Type RequestedFeatureLevel;
    const TCHAR* LoadedRHIModuleName;
    IDynamicRHIModule* DynamicRHIModule = LoadDynamicRHIModule(RequestedFeatureLevel, LoadedRHIModuleName);

    if (DynamicRHIModule)
    {
        // Create the dynamic RHI.
        DynamicRHI = DynamicRHIModule->CreateRHI(RequestedFeatureLevel);
    }

    return DynamicRHI;
}


TEST(TestCaseName, TestName)
{
    
    GDynamicRHI = DynamicRHIModule->CreateRHI();

    // or this
    GDynamicRHI = PlatformCreateDynamicRHI();

    // followed by
    GDynamicRHI->Init();    

    
}

*/