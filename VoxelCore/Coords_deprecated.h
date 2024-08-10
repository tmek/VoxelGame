#pragma once
#include "ChunkKey.h"
#include "Conversions.h"
#include "Coords2.h"
#include "VoxelDefines.h"

/*
    note 1d storage order of blocks in a chunk:
    -------------------------------------------
    current choice: XZY order.
    - as block index increases, the x-coordinate increases fastest, then the z-coordinate, then the y-coordinate.
    - this affects functions that convert between 3D and 1D coordinates.
    - nested loops that iterate over blocks in a chunk should iterate in the order of y, z, x.
    - this order is chosen because it is the most cache-friendly for the voxel engine's use cases.

    for( int y = 0; y < CHUNK_SIZE_Y; ++y ) // loop over stacked layers of 16x16
    {
        for( int z = 0; z < CHUNK_SIZE_Z; ++z ) // loop over the 16 rows in a layer (increase with depth) 
        {
            for( int x = 0; x < CHUNK_SIZE_X; ++x ) // loop over the 16 blocks in a row (increase with width)
            {
                // do something with block at (x, y, z)
            }
        }
    }
*/

typedef int LocalIndex1D;

// A block's local coordinates within a chunk
struct LocalCoords
{
    int x;
    int y;
    int z;
};

// A block's coordinates in the world
struct WorldCoords
{
    int x;
    int y;
    int z;
};

// A 3D region of blocks in the world
struct WorldRegion
{
    WorldCoords minCoords;
    WorldCoords maxCoords;
};

// A block represented as a chunk key and local 1D index
struct BlockAddress
{
    ChunkKey chunkKey;
    LocalIndex1D localIndex1D;
};

// Convert world coordinates to a local coordinate within a chunk.
inline int WorldToLocal_deprecated(const int worldCoord1D, const int SIZE)
{
    // todo: WorldToLocal would be accessed in tight inner loops, i don't like that it has a branch.
    //       we could use a mask instead of a branch.       

    int local = worldCoord1D % SIZE;

    // Adjust for negative coordinates without a branch
    // https://chatgpt.com/c/5984c0a2-c019-4421-87a8-b23a48ec47c4
    // Use bitwise operation to account for negative world coordinates
    //local = (local + SIZE) & (SIZE - 1);
    // use this method if SIZE is not a power of 2
    local += (local < 0) * SIZE;
    // note: both of these might be something the compiler would have done anyway.
    // should compare profiles of both methods vs the branch method. 

    return local;
}


// LocalCoords to Index1D in XZY Order conversion
inline LocalIndex1D LocalCoordsToIndex1DXZY(const LocalCoords& localCoords)
{
    constexpr int BLOCK_SIZE = 1;
    constexpr int ROW_SIZE = CHUNK_SIZE_X;
    constexpr int LAYER_SIZE = CHUNK_SIZE_X * CHUNK_SIZE_Z;

    // x index increases fastest, then z, then y
    int xindex = localCoords.x * BLOCK_SIZE;
    int zindex = localCoords.z * ROW_SIZE;
    int yindex = localCoords.y * LAYER_SIZE;

    LocalIndex1D index = xindex + zindex + yindex;

    // todo: this needs to be check() macro or something that gets removed in test/shipping
    if (index < 0 || index >= CHUNK_TOTAL_BLOCKS)
    {
        __debugbreak();
    }

    return index;
}


// Index1D in XZY Order to LocalCoords conversion
inline LocalCoords Index1DXZYToLocalCoords(LocalIndex1D index)
{
    LocalCoords localCoords;
    localCoords.x = index % CHUNK_SIZE_X; // Correct: Modulo by CHUNK_SIZE_X gives the x-coordinate
    localCoords.z = (index / CHUNK_SIZE_X) % CHUNK_SIZE_Z;
    // Correct: Divide by CHUNK_SIZE_X, then modulo by CHUNK_SIZE_Z for z-coordinate
    localCoords.y = index / (CHUNK_SIZE_X * CHUNK_SIZE_Z);
    // Corrected: Divide by the product of CHUNK_SIZE_X and CHUNK_SIZE_Z for y-coordinate
    return localCoords;
}


// World coordinates to chunk key conversion
inline ChunkKey WorldCoordsToChunkKey(const int worldX, const int worldZ)
{
    return {worldX / CHUNK_SIZE_X, worldZ / CHUNK_SIZE_Z};
}

inline ChunkKey WorldCoordsToChunkKey(const WorldCoords& worldCoords)
{
    return {worldCoords.x / CHUNK_SIZE_X, worldCoords.z / CHUNK_SIZE_Z};
}


// World coordinates to block address conversion
inline BlockAddress WorldCoordsToBlockAddress(const WorldCoords& worldCoords)
{
    WorldBlock worldBlock = {worldCoords.x, worldCoords.y, worldCoords.z};
    
    // Calculate local coordinates
    //LocalCoords localCoords;
    // localCoords.x = WorldToLocal2(worldCoords.x, CHUNK_SIZE_X);
    // localCoords.y = WorldToLocal2(worldCoords.y, CHUNK_SIZE_Y);
    // localCoords.z = WorldToLocal2(worldCoords.z, CHUNK_SIZE_Z);
    

    BlockAddress blockAddress;
    blockAddress.chunkKey = WorldCoordsToChunkKey(worldCoords);
    blockAddress.localIndex1D = LocalCoordsToIndex1DXZY(localCoords);

    return blockAddress;
}

inline BlockAddress WorldCoordsToBlockAddress(const int worldX, const int worldY, const int worldZ)
{
    BlockAddress blockAddress;

    // Calculate chunk coordinates
    blockAddress.chunkKey.X = worldX / CHUNK_SIZE_X;
    blockAddress.chunkKey.Z = worldZ / CHUNK_SIZE_Z;

    // Calculate local coordinates
    LocalCoords localCoords;
    localCoords.x = WorldToLocal(worldX, CHUNK_SIZE_X);
    localCoords.y = WorldToLocal(worldY, CHUNK_SIZE_Y);
    localCoords.z = WorldToLocal(worldZ, CHUNK_SIZE_Z);

    blockAddress.localIndex1D = LocalCoordsToIndex1DXZY(localCoords);

    return blockAddress;
}


// inline void GetChunkOrigin(ChunkKey chunkKey)
// {
//     return {chunkKey.X * CHUNK_SIZE_X, 0, chunkKey.Z * CHUNK_SIZE_Z};
// }

// inline WorldCoords LocalToWorldCoords(const ChunkKey& chunkKey, int localX, int localY, int localZ)
// {
//     WorldCoords chunkWorld = GetChunkOrigin(chunkKey);
//     return {chunkWorld.x + localX, chunkWorld.y + localY, chunkWorld.z + localZ};
// }


/*
    World
    - The game world is made up of 1x1 meter blocks.

    Chunks
    - Blocks are stored in chunks.
    - Each chunk is 16x384x16 blocks (98,304 blocks in total, 288 KB of memory at 3 bytes per block). 

    Blocks
    - Each block is represented by 2 bytes `BlockState` and 1 byte for `LightLevels`


    Coordinates
    - Coordinate structures and conversions are extremely important in this type of system.

    spaces:
        world space (wx, wy, wz)
        local space (chunk key and local coordinates: ckx, cky, ckz, lx, ly, lz)
 
    coordinates and indexes
        block world coordinates (bwx, bwy, bwz) in world space
        block local coordinates (blx, bly, blz) in chunk space
        block index (bi) in chunk space

        chunk origin (cwx, cwy, cwz) in world space
        chunk key (cix, ckz)

    conversions:
        BlockWorldToBlockLocal
        BlockLocalToBlockWorld (requires chunk origin or chunk key)

        BlockLocalToBlockIndex
        BlockIndexToBlockLocal

        BlockIndexToBlockWorld (requires chunk origin or chunk key)
        BlockWorldToChunkKey
        BlockWorldToChunkOrigin
        
    signatures:
        
        
        



        


        
 */

struct BlockCoords
{
    int X;
    int Y;
    int Z;
};


// converts world block coordinates to chunk key and local block coordinates
inline void WorldBlockToLocal(
    const int& wbxIn, const int& wbyIn, const int& wbzIn,
    int& ckxOut, int& ckzOut,
    int& lbxOut, int& lbyOut, int& lbzOut)
{
    // convert world block coordinates to chunk key and local block coordinates
    ckxOut = wbxIn / CHUNK_SIZE_X;
    ckzOut = wbzIn / CHUNK_SIZE_Z;
    if (wbxIn < 0) ckxOut--;
    if (wbzIn < 0) ckzOut--;

    // ACOUNT FOR NEGATIVE COORDINATES
    // the compiler will probably optimize this the best way but
    // ideally this would be branchless and low cost ops like the bitwise ops below (which don't seem to work)
    // ckxOut = (wbxIn + ((wbxIn >> 31) & (CHUNK_SIZE_X - 1))) / CHUNK_SIZE_X; // these didn't work
    // ckzOut = (wbzIn + ((wbzIn >> 31) & (CHUNK_SIZE_Z - 1))) / CHUNK_SIZE_Z;

    lbxOut = WorldToLocal(wbxIn, CHUNK_SIZE_X);
    lbyOut = wbyIn;
    lbzOut = WorldToLocal(wbzIn, CHUNK_SIZE_Z);

    if (!(lbyOut >= 0 && lbyOut < CHUNK_SIZE_Y)) __debugbreak();
    if (!(lbxOut >= 0 && lbxOut < CHUNK_SIZE_X)) __debugbreak();
    if (!(lbzOut >= 0 && lbzOut < CHUNK_SIZE_Z)) __debugbreak();
}

inline void LocalBlockToWorld(
    const int& ckxIn, const int& ckzIn,
    const int& lbxIn, const int& lbyIn, const int& lbzIn,
    int& wbxOut, int& wbyOut, int& wbzOut)
{
    // convert chunk key and local block coordinates to world block coordinates
    wbxOut = ckxIn * CHUNK_SIZE_X + lbxIn;
    wbyOut = lbyIn;
    wbzOut = ckzIn * CHUNK_SIZE_Z + lbzIn;
}

inline void WorldBlockToLocal(const BlockCoords& worldBlockCoordsIn, ChunkKey& chunkKeyOut,
                              BlockCoords& localBlockCoordsOut)
{
    WorldBlockToLocal(
        worldBlockCoordsIn.X, worldBlockCoordsIn.Y, worldBlockCoordsIn.Z,
        chunkKeyOut.X, chunkKeyOut.Z,
        localBlockCoordsOut.X, localBlockCoordsOut.Y, localBlockCoordsOut.Z);
}

inline void LocalBlockToWorld(const ChunkKey& chunkKeyIn, const BlockCoords& localBlockCoordsIn,
                              BlockCoords& worldBlockCoordsOut)
{
    LocalBlockToWorld(
        chunkKeyIn.X, chunkKeyIn.Z,
        localBlockCoordsIn.X, localBlockCoordsIn.Y, localBlockCoordsIn.Z,
        worldBlockCoordsOut.X, worldBlockCoordsOut.Y, worldBlockCoordsOut.Z);
}

/*
    WorldBlockToLocal
    LocalBlockToWorld

    ChunkKeyToWorld
    WorldToChunkKey
*/
