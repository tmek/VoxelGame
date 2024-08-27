// Copyright

#pragma once

#include <bit>
#include "Voxel/VoxelTypes.h"

#include "Generated/VoxelCoreExports.h"

// aliased field types in case we want to experiment
struct Block;
using BlockType = uint16;
using BlockProperty = uint8;
using BlockLightLevel = uint8;
using BlockRef = Block&;

/** Special blocks that are always included in the block types table. */
enum class ReservedBlocks : BlockType
{
    Air = 0,
    Stone = 1,
    Water = 13,
    // Add more reserved types here if needed
};

constexpr uint32 CastBlockTypeToBlock(BlockType type) noexcept
{
    return std::bit_cast<uint16>(type);
}

/** 
 * A block is a single unit of the world. 
 * It has a type, rotation, flipped state, visibility state, solidity state, transparency state, liquid state, light source state, light level, and sky light level.
 */
struct alignas(4) VOXELCORE_API Block final
{
    /** Block type */
    BlockType TypeIndex;              // runtime index into the block types table. (0-65535). a few special index values are reserved for air=0, stone=1, water=13, etc. the rest are configurable depending on user mods etc.

    /** Block properties */
    BlockProperty RotationIndexY : 2;    // Multiply by 90 degrees for rotation angle around the Y-axis. (0-3)
    BlockProperty IsFlipped      : 1;    // Flipped vertically
    BlockProperty IsVisible      : 1;
    BlockProperty IsSolid        : 1;
    BlockProperty IsTransparent  : 1;
    BlockProperty IsLiquid       : 1;
    BlockProperty IsLightSource  : 1;

    /** Light levels */
    BlockLightLevel LightLevel    : 4;    // light level for this block (0-15)
    BlockLightLevel SkyLightLevel : 4;    // Sky light level for this block (0-15)

    // The 'EmptyBlock' is a special block typically used to represent
    // any block in the world that is not part of a loaded chunk.
    static const Block Empty;

    /** Utility */
    [[nodiscard]] constexpr float GetRotationYAngle() const noexcept { return static_cast<float>(RotationIndexY) * 90.0f; }
    [[nodiscard]] constexpr bool IsAir() const noexcept { return TypeIndex == static_cast<BlockType>(ReservedBlocks::Air); }
    [[nodiscard]] constexpr bool IsWater() const noexcept { return TypeIndex == static_cast<BlockType>(ReservedBlocks::Water); }

    /** Interoperability with uint32 */
    // note: If we enable these (or a default constructor), `Block block;` will initialize to zero. Without these, it will be uninitialized. Test performance of both ways.
    constexpr Block(uint32 RawData = 0) noexcept { *this = std::bit_cast<Block>(RawData); }
    constexpr operator uint32() const noexcept { return std::bit_cast<uint32>(*this); }
};
static_assert(sizeof(Block) == 4, "Block size is not 4 bytes");
static_assert(alignof(Block) == 4, "Block alignment is not 4 bytes");
