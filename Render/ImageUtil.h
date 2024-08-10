#pragma once

#include <vector>

typedef unsigned int UINT;
typedef unsigned char BYTE;

#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <fstream>
#include <iostream>

using Microsoft::WRL::ComPtr;

#pragma pack(push, 1)
struct BMPHeader
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BMPInfoHeader
{
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)


class ImageUtil
{
public:
    static void               CreateNoisePattern(UINT width, UINT height, std::vector<UINT>& data);
    static void CreateCheckerboardPattern(UINT width, UINT height, std::vector<UINT>& data);
};
