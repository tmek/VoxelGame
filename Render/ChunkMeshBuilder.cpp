#include "ChunkMeshBuilder.h"

#include <memory>

#include "BlockMeshBuilder.h"
#include "VoxelCore/Chunk.h"
#include "WorldGen/WorldOperations.h"


extern VoxelWorld GWorld;

static DirectX::XMFLOAT4 TintColors[] = {
    {0.5f, 0.5f, 0.5f, 1.0f}, // stone 1
    {0.55f, 0.27f, 0.07f, 1.0f}, // dirt
    {0.3f, 0.6f, 0.2f, 1.0f}, // grass
    {0.0f, 0.5f, 1.0f, 1.0f}, // water
    {0.2f, 0.2f, 0.2f, 1.0f}, // bedrock
    {0.057f, 0.392f, 0.155f, 1.0f}, // tree leaves 6
     {0.95f, 0.95f, 0.95f, 1.0f}, // snow
};

inline XMFLOAT4 GetBlockColor(int blockType)
{
    if(blockType < 1 || blockType > 7)
    {
        return TintColors[5-1];
    }
    return TintColors[blockType-1];
}

Mesh ChunkMeshBuilder::Build(const ChunkKey& chunkKey, const Chunk& chunk, ID3D11Device* device)
{
    WorldOperations world(GWorld);
    MeshBuilder<PosColNormTexVertex> builder(device);
    BlockMeshBuilder blockMeshBuilder(builder);
    
    // get chunk origin in world space
    WorldBlockCoord chunkOrigin;
    ChunkToWorld(chunkKey, chunkOrigin);

    // loop through all blocks in the chunk in yzx order
    for (int localY = 0; localY < CHUNK_SIZE_Y; ++localY)
        for (int localZ = 0; localZ < CHUNK_SIZE_Z; ++localZ)
            for (int localX = 0; localX < CHUNK_SIZE_X; ++localX)
            {
                // get world coords for block
                WorldBlockCoord worldBlock = {chunkOrigin.X + localX, chunkOrigin.Y + localY, chunkOrigin.Z + localZ};

                LocalBlockCoord localBlock;
                WorldToLocal(worldBlock, localBlock);

                if (chunk.IsAirBlock(localBlock.Index))
                {
                    continue;
                }

                // get block type
                BlockType blockType = chunk.GetBlockType(localBlock.Index);
                
                // set position and color for this block
                XMFLOAT4 BlockColor = GetBlockColor(blockType);
                blockMeshBuilder.SetMeshRelativeBlockOrigin((float)localBlock.X, (float)localBlock.Y, (float)localBlock.Z);
                blockMeshBuilder.SetColor(BlockColor.x, BlockColor.y, BlockColor.z, BlockColor.w);

                // determine which faces of this block are exposed to air
                bool IsFaceExposedToAirNegX = world.IsAirBlock(worldBlock.X - 1, worldBlock.Y, worldBlock.Z); // left
                bool IsFaceExposedToAirNegY = world.IsAirBlock(worldBlock.X, worldBlock.Y - 1, worldBlock.Z); // down
                bool IsFaceExposedToAirNegZ = world.IsAirBlock(worldBlock.X, worldBlock.Y, worldBlock.Z - 1); // backward                   
                bool IsFaceExposedToAirPosX = world.IsAirBlock(worldBlock.X + 1, worldBlock.Y, worldBlock.Z); // right
                bool IsFaceExposedToAirPosY = world.IsAirBlock(worldBlock.X, worldBlock.Y + 1, worldBlock.Z); // up
                bool IsFaceExposedToAirPosZ = world.IsAirBlock(worldBlock.X, worldBlock.Y, worldBlock.Z + 1); // forward

                // add air exposed faces to the mesh
                if (IsFaceExposedToAirPosX) blockMeshBuilder.AppendPosXFace();
                if (IsFaceExposedToAirPosY) blockMeshBuilder.AppendPosYFace();
                if (IsFaceExposedToAirPosZ) blockMeshBuilder.AppendPosZFace();
                if (IsFaceExposedToAirNegX) blockMeshBuilder.AppendNegXFace();
                if (IsFaceExposedToAirNegY) blockMeshBuilder.AppendNegYFace();
                if (IsFaceExposedToAirNegZ) blockMeshBuilder.AppendNegZFace();
            }

    Mesh mesh = builder.ToMesh();

    // store the chunk key in the mesh for debugging
    mesh.DebugTag = std::make_shared<std::string>(
        "ChunkMesh: " + std::to_string(chunkKey.X) + ", " + std::to_string(chunkKey.Z));

    return mesh;

    // Block data queried:
    // + every block in the chunk
    // + all blocks touching the chunk
    // Block data written:
    // + none
    // todo: this should be a pattern when working with world data.
    // perhaps we can make a class that does this for us called WorldDataQuery
    // and another class called WorldDataEdit (read/write)    
}
