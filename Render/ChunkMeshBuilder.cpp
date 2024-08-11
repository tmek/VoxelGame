#include "ChunkMeshBuilder.h"

#include <memory>

#include "BlockMeshBuilder.h"
#include "VoxelCore/Chunk.h"
#include "WorldGen/BlockTypes.h"
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
    BlockColor color = BlockColors[BEDROCK];

    if (blockType > AIR && blockType <= BAMBOO)
    {
        color = BlockColors[blockType];
    }

    auto result = XMFLOAT4(color.Red / 255.0f, color.Green / 255.0f, color.Blue / 255.0f, 1.0f);

    // copy channels to floats
    float r = (float)color.Red / 255.0f;
    float g = (float)color.Green / 255.0f;
    float b = (float)color.Blue / 255.0f;

    // desaturate color a bit
    float desaturation = 0.3f;
    r = r + (1.0f - r) * desaturation;
    g = g + (1.0f - g) * desaturation;
    b = b + (1.0f - b) * desaturation;

    // darken a bit
    float darkness = 0.7f;
    r = r * darkness;
    g = g * darkness;
    b = b * darkness;

    // return result
    return XMFLOAT4(r, g, b, 1.0f);
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
                bool IsWaterBlock = blockType == WATER;

                // determine which faces to include:
                //  if i'm water and touching air, add faces
                //  if i'm solid and face is touching air or water, add face

                bool includePosX;
                bool includePosY;
                bool includePosZ;
                bool includeNegX;
                bool includeNegY;
                bool includeNegZ;

                // is face touching to air?  
                bool IsFaceTouchingToAirNegX = world.IsAirBlock(worldBlock.X - 1, worldBlock.Y, worldBlock.Z); // left
                bool IsFaceTouchingToAirNegY = world.IsAirBlock(worldBlock.X, worldBlock.Y - 1, worldBlock.Z); // down
                bool IsFaceTouchingToAirNegZ = world.IsAirBlock(worldBlock.X, worldBlock.Y, worldBlock.Z - 1);
                // backward                   
                bool IsFaceTouchingToAirPosX = world.IsAirBlock(worldBlock.X + 1, worldBlock.Y, worldBlock.Z); // right
                bool IsFaceTouchingToAirPosY = world.IsAirBlock(worldBlock.X, worldBlock.Y + 1, worldBlock.Z); // up
                bool IsFaceTouchingToAirPosZ = world.IsAirBlock(worldBlock.X, worldBlock.Y, worldBlock.Z + 1);
                // forward

                if (IsWaterBlock)
                {
                    includePosX = IsFaceTouchingToAirPosX;
                    includePosY = IsFaceTouchingToAirPosY;
                    includePosZ = IsFaceTouchingToAirPosZ;
                    includeNegX = IsFaceTouchingToAirNegX;
                    includeNegY = IsFaceTouchingToAirNegY;
                    includeNegZ = IsFaceTouchingToAirNegZ;
                }
                else
                {
                    // is face touching water?
                    bool IsFaceTouchingToWaterNegX = world.IsWaterBlock(worldBlock.X - 1, worldBlock.Y, worldBlock.Z);
                    // left
                    bool IsFaceTouchingToWaterNegY = world.IsWaterBlock(worldBlock.X, worldBlock.Y - 1, worldBlock.Z);
                    // down
                    bool IsFaceTouchingToWaterNegZ = world.IsWaterBlock(worldBlock.X, worldBlock.Y, worldBlock.Z - 1);
                    // backward                   
                    bool IsFaceTouchingToWaterPosX = world.IsWaterBlock(worldBlock.X + 1, worldBlock.Y, worldBlock.Z);
                    // right
                    bool IsFaceTouchingToWaterPosY = world.IsWaterBlock(worldBlock.X, worldBlock.Y + 1, worldBlock.Z);
                    // up
                    bool IsFaceTouchingToWaterPosZ = world.IsWaterBlock(worldBlock.X, worldBlock.Y, worldBlock.Z + 1);
                    // forward

                    includePosX = IsFaceTouchingToAirPosX || IsFaceTouchingToWaterPosX;
                    includePosY = IsFaceTouchingToAirPosY || IsFaceTouchingToWaterPosY;
                    includePosZ = IsFaceTouchingToAirPosZ || IsFaceTouchingToWaterPosZ;
                    includeNegX = IsFaceTouchingToAirNegX || IsFaceTouchingToWaterNegX;
                    includeNegY = IsFaceTouchingToAirNegY || IsFaceTouchingToWaterNegY;
                    includeNegZ = IsFaceTouchingToAirNegZ || IsFaceTouchingToWaterNegZ;
                }

                // set position and color for this block
                blockMeshBuilder.SetMeshRelativeBlockOrigin((float)localBlock.X, (float)localBlock.Y,
                                                            (float)localBlock.Z);

                // give each face a slightly different ambient shade
                XMFLOAT4 BlockColor = GetBlockColor(blockType);
                BlockColor.w = IsWaterBlock ? 0.7f : 1.0f;

                constexpr float TopFaceAmbientShade = 1.0f;
                constexpr float BottomFaceAmbientShade = 0.5f;
                constexpr float EastWestFaceAmbientShade = 0.8f;
                constexpr float NorthSouthFaceAmbientShade = 0.6f;

                // top face
                blockMeshBuilder.SetColor(BlockColor.x * TopFaceAmbientShade, BlockColor.y * TopFaceAmbientShade,
                                        BlockColor.z * TopFaceAmbientShade, BlockColor.w);
                if(includePosY) blockMeshBuilder.AppendPosYFace();

                // bottom face
                blockMeshBuilder.SetColor(BlockColor.x * BottomFaceAmbientShade, BlockColor.y * BottomFaceAmbientShade,
                                        BlockColor.z * BottomFaceAmbientShade, BlockColor.w);
                if(includeNegY) blockMeshBuilder.AppendNegYFace();

                // east/west faces
                blockMeshBuilder.SetColor(BlockColor.x * EastWestFaceAmbientShade, BlockColor.y * EastWestFaceAmbientShade,
                                        BlockColor.z * EastWestFaceAmbientShade, BlockColor.w);
                if(includePosX) blockMeshBuilder.AppendPosXFace();
                if(includeNegX) blockMeshBuilder.AppendNegXFace();

                // north/south faces
                blockMeshBuilder.SetColor(BlockColor.x * NorthSouthFaceAmbientShade, BlockColor.y * NorthSouthFaceAmbientShade,
                                        BlockColor.z * NorthSouthFaceAmbientShade, BlockColor.w);
                if(includePosZ) blockMeshBuilder.AppendPosZFace();
                if(includeNegZ) blockMeshBuilder.AppendNegZFace();

            } // end inner block loop

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
