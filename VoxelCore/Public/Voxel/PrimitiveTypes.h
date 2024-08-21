// Copyright

#pragma once

#include "CoreTypes.h"

/** Signed integers */
// using int8 = int8_t;
// using int16 = int16_t;
// using int32 = int32_t;
// using int64 = int64_t;
//
// /** Usigned integers */
// using uint8 = uint8_t;
// using uint16 = uint16_t;
// using uint32 = uint32_t;
// using uint64 = uint64_t;

/** 3D vector */ 
template<typename T>
struct TVector3
{
    T X;
    T Y;
    T Z;
}; 

/** 3D Region */
// template<typename T>
// struct Region
// {
//     TVector3<T> Min;
//     TVector3<T> Max;
// };

/** Blocks */ 
using BlockCoordinate = TVector3<int32>;
using BlockIndex = uint32;

/** A 3D region of blocks in the world, or within a chunk as block offsets. */
struct BlockRegion
{
    BlockCoordinate Min;
    BlockCoordinate Max;
};

