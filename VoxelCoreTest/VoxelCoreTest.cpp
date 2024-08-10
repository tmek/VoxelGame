// voxel
#include "VoxelCore/ChunkKey.h"
#include "VoxelCore/Chunk.h"
#include "VoxelCore/VoxelWorld.h"

// asserts
#include "Asserts.h"

// stdlib
#include <cstdio>

#include "VoxelCore/Conversions.h"
#include "WorldGen/WorldOperations.h"

void TestChunk()
{
    int BytesPerBlock = 2 + 1; // 16 bytes for blockStates, 8 bytes for lightLevels
    AssertAreEqual(sizeof(Chunk), BytesPerBlock * CHUNK_TOTAL_BLOCKS,
        "Chunk size is %d bytes", BytesPerBlock * CHUNK_TOTAL_BLOCKS);

    // create a chunk
    Chunk chunk;

    // read and write a block
    BlockIndex index = 8;
    chunk.SetBlockType(index, 1u);
    uint16_t type = chunk.GetBlockType(index);

    AssertAreEqual(type, 1, "block type == 1");
}

VoxelWorld world;

void ChunkBugTest()
{
    // int x = -52;
    // int y = 10;
    // int z = 1;
    WorldBlockCoord worldBlock = {-52, 10, 1}; 
    BlockType blockType = 3;
    
    // Get the chunk
    //BlockAddress ba = WorldCoordsToBlockAddress({x, y, z});
    LocalBlockCoord localBlock;
    WorldToLocal(worldBlock, localBlock);
    Chunk& chunk = world.GetChunk(localBlock.chunkKey);

    // Set the block type
    chunk.SetBlockType(localBlock.Index, blockType);    
}

void TestMultipleChunks()
{
    ChunkBugTest();

    //return;
    
    // get a chunk and read/write a block
    ChunkKey key = {1, 1};
    Chunk& chunk = world.GetChunk(key);

    // get a chunk with negative coordinates
    ChunkKey key2 = {-64, -64};
    Chunk& chunk2 = world.GetChunk(key2);

    ChunkKeyHash hashFunction;
    size_t hash = hashFunction(key2);
    
    chunk.SetBlockType(3, 3);
    int blocktype = chunk.GetBlockType(3);

    AssertAreEqual(blocktype, 3, "blocktype == 3");

    // loop through multiple chunks and read/write several blocks
    for (int i = 0; i < 10; i++)
    {
        ChunkKey key = {i, i};
        Chunk& chunk = world.GetChunk(key);

        // read/write multiple blocks
        for (int j = 0; j < 10; j++)
        {
            chunk.SetBlockType(j, j);
            int blocktype = chunk.GetBlockType(j);

            AssertAreEqual(blocktype, j, "chunk: %d, blocktype: %d==%d", i, blocktype, j);
        }
    }
}


void TestWorld()
{
    VoxelWorld gworld;

    // set block
    Chunk& chunk = gworld.GetChunk({0,0});
    LocalBlockCoord localBlock;
    WorldToLocal({0,0,0}, localBlock);
    chunk.SetBlockType(localBlock.Index, 1);

    // assert localindex is correct
    AssertAreEqual(localBlock.Index, 0, "localIndex1D == 0");
    
    // retrieve it
    auto result = chunk.GetBlockType(localBlock.Index);

    // assert block type is correct
    AssertAreEqual(result, 1, "block type == 1");

    
}

void TestConversions()
{
    WorldBlockCoord worldBlock = {15, 8, 3};
    LocalBlockCoord localBlock;
    WorldToLocal(worldBlock, localBlock);
    AssertAreEqual(localBlock.chunkKey.X, 0, "chunk key x == 0");
    AssertAreEqual(localBlock.chunkKey.Z, 0, "chunk key z == 0");
    AssertAreEqual(localBlock.X, 15, "local block x == 15");
    AssertAreEqual(localBlock.Y, 8, "local block y == 8");
    AssertAreEqual(localBlock.Z, 3, "local block z == 3");
    AssertAreEqual(localBlock.Index, 2111, "local block index1D == 2111");

    worldBlock = {16,17,18};
    WorldToLocal(worldBlock, localBlock);
    AssertAreEqual(localBlock.chunkKey.X, 1, "chunk key x == 1");
    AssertAreEqual(localBlock.chunkKey.Z, 1, "chunk key z == 1");
    AssertAreEqual(localBlock.X, 0, "local block x == 0");
    AssertAreEqual(localBlock.Y, 17, "local block y == 17");
    AssertAreEqual(localBlock.Z, 2, "local block z == 2");
    AssertAreEqual(localBlock.Index, 4384, "local block index1D == 4384");

    worldBlock =  {-1,0,0}; // -y should not be allowed this should fail
    WorldToLocal(worldBlock, localBlock);
    AssertAreEqual(localBlock.chunkKey.X, -1, "chunk key x == -1");
    AssertAreEqual(localBlock.chunkKey.Z, 0, "chunk key z == 0");
    AssertAreEqual(localBlock.X, 15, "local block x == 15");
    AssertAreEqual(localBlock.Y, 0, "local block y == 0");
    AssertAreEqual(localBlock.Z, 0, "local block z == 0");
    AssertAreEqual(localBlock.Index, 15, "local block index1D == 15");

    worldBlock =  {-1,2,-3}; // -y should not be allowed this should fail
    WorldToLocal(worldBlock, localBlock);
    AssertAreEqual(localBlock.chunkKey.X, -1, "chunk key x == -1");
    AssertAreEqual(localBlock.chunkKey.Z, -1, "chunk key z == -1");
    AssertAreEqual(localBlock.X, 15, "local block x == 15");
    AssertAreEqual(localBlock.Y, 2, "local block y == 2");
    AssertAreEqual(localBlock.Z, 13, "local block z == 13");
    AssertAreEqual(localBlock.Index, 735, "local block index1D == 735");

}

int main(int argc, char* argv[])
{
    // disable stdout buffering
    setvbuf(stdout, NULL, _IONBF, 0);
    // disable stderr buffering
    //setvbuf(stderr, NULL, _IONBF, 0);

    // test stdout and stderr
    fprintf(stderr, "stderr test\n");
    fprintf(stdout, "stdout test\n");

    BeginTestGroup("Sanity Check");
    AssertAreEqual(1, 1, "1 == 1");
    
    TestConversions();
    return 0;

    BeginTestGroup("Chunk Tests");
    TestChunk();

    BeginTestGroup("World Tests");
    TestMultipleChunks();
    TestMultipleChunks();

    TestWorld();

    TestConversions();
   
    return 0;
}
