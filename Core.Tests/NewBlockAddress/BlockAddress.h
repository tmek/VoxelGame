// // Copyright (c) 2024 Thomas. All rights reserved.

#pragma once


using int32 = unsigned int;

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;

constexpr uint32 CHUNK_WIDTH = 16;
constexpr uint32 CHUNK_DEPTH = 16;
constexpr uint32 CHUNK_HEIGHT = 384;

constexpr uint32 BLOCKS_PER_CHUNK = CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT;
constexpr uint32 BLOCKS_PER_LAYER = CHUNK_WIDTH * CHUNK_DEPTH;
constexpr uint32 BLOCKS_PER_ROW = CHUNK_WIDTH;

/*  ----------------------------------------------------------------------------
    current status of block address:

    It's very cool that it can automatically convert between 1D Index and 3D coordinates.
    - Reading and writing the Index is free
    - Reading/Writing the x,y,z bitfields has some minor cost (shifts and masks etc.)
        - some definite downsides to this, you wouldn't want to use x,y,z as loop variables for example

    In the end the IndexToOffset and OffsetToIndex functions are so fast and have the ability
    to do range checking, and only execute when you need them,
    that I think the BlockAddress struct is not needed. even though in concept it's really cool
    theres actually work going on under the hood to access the bitfields that make it less performant than it seems.
    ---------------------------------------------------------------------------- */
    
// todo: This is a problem with BlockAddress. You can set X, Z, Y values > 16 and it will just accept them.
//       Now that we have the conversion functions, we can check the values and make sure they are within the range.
//       and they seem very fast, so the novelty of the BlockAddress struct "automatically" converting them is not needed.



/// <summary>
/// Represents a block's address within a chunk, providing automatic conversions 
/// between a 1-Dimensional index and the corresponding 3-Dimensional offsets 
/// within that chunk (Index, Column, Row, Layer; or X, Z, Y).
/// </summary>
struct BlockAddress
{
    // The union is used to allow the Index, Column/Row/Layer, and X/Z/Y fields to be accessed interchangeably.
    // This is useful for setting the index and then reading the individual components, or vice versa.
    // The bitfields 

    union
    {
        /// <summary>
        /// 1-Dimensional index of the block within the chunk in order of width/depth/height.
        /// </summary>
        uint32 Index;

        struct  // NOLINT(clang-diagnostic-nested-anon-types)
        {
            uint32  Column : 4;    // 4 bits for the horizontal position of the block (0-15)
            uint32  Row    : 4;    // 4 bits for the depth position of the block (0-15)
            uint32  Layer  : 24;   // 24 bits for the vertical position of the block, range limited in IsValid() by CHUNK_HEIGHT (0-383)

            // Note: The reason the full remaining 24-bits goes to Layer, rather than using 9-bits or 16-bits plus a hidden
            // padding field, is so we don't have to initialize the padding field to 0.
            // Setting Layer will do it automatically this way. 
        };

        struct // NOLINT(clang-diagnostic-nested-anon-types)
        {
            uint32 X : 4;
            uint32 Z : 4;
            uint32 Y : 24;
        };
    };

    bool IsValid() const
    {
        return Y < CHUNK_HEIGHT;
    }
};
