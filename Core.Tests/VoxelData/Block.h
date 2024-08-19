#pragma once

#include "PrimitiveTypes.h"
#include <bit>

struct VOXELCORE_API Block
{
    uint16 Type;

    // block properties             
    uint8 RotationIndexY : 2; // (0-3) Multiply by 90 degrees for angle.
    bool IsFlippedVertically : 1;
    bool IsVisible : 1;
    bool IsSolid : 1;
    bool IsTransparent : 1;
    bool IsLiquid : 1;
    bool IsLightSource : 1;

    // light levels (0-15)
    uint8 LightLevel : 4;
    uint8 SkyLightLevel : 4;

    // interoperability with uint32
    inline Block(uint32 RawData = 0) { *this = std::bit_cast<Block>(RawData); }
    inline operator uint32() const { return std::bit_cast<uint32>(*this); }
};
