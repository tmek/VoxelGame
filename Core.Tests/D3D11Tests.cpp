#include "../GameCore/StopWatch.h"
#include "Misc/CoreDefines.h"
#include "HAL/PlatformProcess.h"
#include <d3d11.h>
#include "D3D11/PrintDeviceAndContextInfo.h"

#include "gtest/gtest.h"




// Example test case
TEST(D3D11TestSuit, InitD3D11)
{
    printf("D3D11 Test\n");

    // print d3d version we're using
    PrintDirectXVersion(D3D_FEATURE_LEVEL_11_0);

    // print device and output info
    QueryAdaptersAndOutputs();
    
    // create a d3d11 device
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
    UINT createDeviceFlags = 0;
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &device, &featureLevel, &context);

    ASSERT_EQ(hr, S_OK);

    PrintD3D11DeviceInfo(device, context, createDeviceFlags);
    
    // release the device and context
    device->Release();
    context->Release();
}

