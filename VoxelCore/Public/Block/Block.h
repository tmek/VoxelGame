// Copyright

#pragma once

#include "Voxel/PrimitiveTypes.h"
#include "Voxel/VoxelDefines.h"
#include <bit>


// aliased field types in case we want to experiment
struct Block;
using BlockType = uint16;
using BlockProperty = uint8;
using BlockLightLevel = uint8;
using BlockRef = Block&;

#define BlockTypeToBlock(BlockType) std::bit_cast<uint32>(BlockType)


/** 
 * A block is a single unit of the world. 
 * It has a type, rotation, flipped state, visibility state, solidity state, transparency state, liquid state, light source state, light level, and sky light level.
 */
struct alignas(4) VOXELCORE_API Block final
{
    /** Block type */
    BlockType  Type;                        // The block's type. 16-bits: 0-65535

    /** Block properties */
    BlockProperty   RotationIndexY  : 2;    // Multiply by 90 degrees for rotation angle around the Y-axis. (0-3)
    BlockProperty   IsFlipped       : 1;    // Flipped vertically
    BlockProperty   IsVisible       : 1;      
    BlockProperty   IsSolid         : 1;        
    BlockProperty   IsTransparent   : 1;  
    BlockProperty   IsLiquid        : 1;       
    BlockProperty   IsLightSource   : 1;  

    /** Light levels */
    BlockLightLevel LightLevel      : 4;    // light level for this block (0-15)
    BlockLightLevel SkyLightLevel   : 4;    // Sky light level for this block (0-15)

    /** Utility */
    constexpr inline float GetRotationYAngle() const noexcept { return static_cast<float>(RotationIndexY) * 90.0f; }
    constexpr inline bool IsAir() const noexcept { return Type == 0; }
    constexpr inline bool IsWater() const noexcept { return Type == 13; }
    
    /** Interoperability with uint32 */
    // note: If we enable these (or a default constructor), `Block block;` will initialize to zero. Without these, it will be uninitialized. Test performance of both ways.
    constexpr inline Block(uint32 RawData = 0) noexcept { *this = std::bit_cast<Block>(RawData); } 
    constexpr inline operator uint32() const noexcept { return std::bit_cast<uint32>(*this); }
};
static_assert(sizeof(Block) == 4, "Block size is not 4 bytes");
static_assert(alignof(Block) == 4, "Block alignment is not 4 bytes");

inline constexpr Block EmptyBlock = {};
