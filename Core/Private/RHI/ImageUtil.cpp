// copyright

#include <string>

#include "RHI/ImageUtil.h"

#include <fstream>
#include <random>

#include "Misc/AssertionMacros.h"

#include <cstdint>

struct PixelMemoryLayout final
{
    uint32_t RedByteIndex;
    uint32_t GreenByteIndex;
    uint32_t BlueByteIndex;
    uint32_t AlphaByteIndex;
};

constexpr PixelMemoryLayout RGBA_MemoryLayout = {0, 1, 2, 3};
constexpr PixelMemoryLayout BGRA_MemoryLayout = {2, 1, 0, 3};
constexpr PixelMemoryLayout ARGB_MemoryLayout = {1, 2, 3, 0};
constexpr PixelMemoryLayout ABGR_MemoryLayout = {3, 2, 1, 0};

constexpr uint32_t ShiftChannel(uint8_t value, uint32_t byteIndex) noexcept
{
#if PLATFORM_LITTLE_ENDIAN
    return static_cast<uint32_t>(value) << (byteIndex * 8);
#else
    return static_cast<uint32_t>(value) << ((3 - byteIndex) * 8);
#endif
}

constexpr uint32_t PackPixel(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha, const PixelMemoryLayout& layout) noexcept
{
    return ShiftChannel(Red, layout.RedByteIndex) |
           ShiftChannel(Green, layout.GreenByteIndex) |
           ShiftChannel(Blue, layout.BlueByteIndex) |
           ShiftChannel(Alpha, layout.AlphaByteIndex);
}




// // Constants for bit offsets
// constexpr uint32 BGRA_BlueBitOffset = 0;
// constexpr uint32 BGRA_GreenBitOffset = 8;
// constexpr uint32 BGRA_RedBitOffset = 16;
// constexpr uint32 BGRA_AlphaBitOffset = 24;
//
// // todo: I'm pretty sure I named this correctly, but I should double check
// //       I Should study/add wiki on pixel byte orders and platform endianness
//
// #define PACK_BGRA(Red, Green, Blue, Alpha) \
//     ( ((Red) << BGRA_RedBitOffset) | ((Green) << BGRA_GreenBitOffset) | ((Blue) << BGRA_BlueBitOffset) | ((Alpha) << BGRA_AlphaBitOffset) )


void ImageUtil::FillImageWithNoise(const uint32 ImageWidth, const uint32 ImageHeight, std::vector<uint32>& ImageDataOut) noexcept
{
    ImageDataOut.resize(ImageWidth * ImageHeight);

    constexpr uint8 MinBrightness = 128;
    constexpr uint8 MaxBrightness = 255;
    constexpr uint8 OpaqueAlpha = 255;

    // setup random number generator
    std::random_device RandomDevice;
    std::mt19937 RandomGenerator(RandomDevice());
    std::uniform_int_distribution<uint32> BrightnessDistribution(MinBrightness, MaxBrightness);

    for (uint32 y = 0; y < ImageHeight; ++y)
    {
        for (uint32 x = 0; x < ImageWidth; ++x)
        {
            uint8 Brightness = BrightnessDistribution(RandomGenerator);
            uint32 Pixel = PackPixel(Brightness, Brightness, Brightness, OpaqueAlpha, RGBA_MemoryLayout);
            size_t PixelIndex = y * ImageWidth + x;
            ImageDataOut[PixelIndex] = Pixel;
        }
    }
}

void ImageUtil::FillImageWithCheckerboardPattern(const uint32 ImageWidth, const uint32 ImageHeight, std::vector<uint32>& ImageDataOut) noexcept
{
    // validate dimensions match data size
    const uint32 ImageDataSize = ImageWidth * ImageHeight;
    check(ImageDataOut.size() == ImageDataSize);

    // resize the output image data
    ImageDataOut.resize(ImageDataSize);

    // fill image data with a checkerboard pattern
    static bool bIsWhite = false;
    for (uint32 y = 0; y < ImageHeight; ++y)
    {
        bIsWhite = !bIsWhite;
        for (uint32 x = 0; x < ImageWidth; ++x)
        {
            bIsWhite = !bIsWhite;
            uint32 Pixel = bIsWhite ? PackPixel(64, 192, 64, 255, RGBA_MemoryLayout) : PackPixel(64, 64, 192, 255, RGBA_MemoryLayout);
            size_t PixelIndex = y * ImageWidth + x;
            ImageDataOut[PixelIndex] = Pixel;
        }
    }
}


#pragma pack(push, 1)
struct BitmapHeader
{
    uint16 Type;
    uint32 Size;
    uint16 Reserved1;
    uint16 Reserved2;
    uint32 OffBits;
};

struct BitmapInfoHeader
{
    uint32 Size;
    int32 Width;
    int32 Height;
    uint16 Planes;
    uint16 BitCount;
    uint32 Compression;
    uint32 SizeImage;
    int32 XPelsPerMeter;
    int32 YPelsPerMeter;
    uint32 ClrUsed;
    uint32 ClrImportant;
};
#pragma pack(pop)


bool TryLoadBMPFile(const std::string& Filename,
                    uint32& ImageWidthOut, uint32& ImageHeightOut, std::vector<uint8>& ImageDataOut)
{
    // open the file explicitly in binary read mode
    constexpr int32 OpenMode_BinaryReadOnly = std::ios::in | std::ios::binary;
    std::ifstream BinaryFile(Filename, OpenMode_BinaryReadOnly);
    if (!BinaryFile.is_open())
    {
        return false;
    }

    // read in the Header and InfoHeader
    BitmapHeader Header;
    BitmapInfoHeader InfoHeader;
    BinaryFile.read(reinterpret_cast<char*>(&Header), sizeof(Header));
    BinaryFile.read(reinterpret_cast<char*>(&InfoHeader), sizeof(InfoHeader));
    constexpr uint32 BitmapTypeId = 0x4D42;
    if (Header.Type != BitmapTypeId)
    {
        return false;
    }

    // headers are valid, read in the image data
    ImageDataOut.resize(InfoHeader.SizeImage);
    BinaryFile.seekg(Header.OffBits, std::ios::beg);
    BinaryFile.read(reinterpret_cast<char*>(ImageDataOut.data()), ImageDataOut.size());
    ImageWidthOut = InfoHeader.Width;
    ImageHeightOut = InfoHeader.Height;

    return true;
}
