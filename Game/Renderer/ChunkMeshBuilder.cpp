﻿// Copyright

#include <memory>

// core
#include "RHI/MeshAssembler.h"
#include "Utils/ChunkCluster.h"

// game/render
#include "ChunkMeshBuilder.h"
#include "BlockMeshBuilder.h"

// game
#include "CoreGlobals.h"

#include "HAL/PlatformWindow.h"

#include "World/VoxelWorld.h"

#include "WorldGen/BlockTypes.h"
#include "WorldGen/WorldOperations.h"



extern std::shared_ptr<VoxelWorld> GWorld;

// static DirectX::XMFLOAT4 TintColors[] = {
//     {0.5f, 0.5f, 0.5f, 1.0f}, // stone 1
//     {0.55f, 0.27f, 0.07f, 1.0f}, // dirt
//     {0.3f, 0.6f, 0.2f, 1.0f}, // grass
//     {0.0f, 0.5f, 1.0f, 1.0f}, // water
//     {0.2f, 0.2f, 0.2f, 1.0f}, // bedrock
//     {0.057f, 0.392f, 0.155f, 1.0f}, // tree leaves 6
//     {0.95f, 0.95f, 0.95f, 1.0f}, // snow
// };

inline XMFLOAT4 GetBlockColor(uint32 blockType)
{
    BlockColor color = BlockColors[BEDROCK];

    if (blockType > AIR && blockType <= BAMBOO)
    {
        color = BlockColors[blockType];
    }

    // copy channels to floats
    // todo: divides can be expensive over hundreds of thousands of blocks, colors should be stored as floats to begin with.
    float r = (float)color.Red / 255.0f;
    float g = (float)color.Green / 255.0f;
    float b = (float)color.Blue / 255.0f;

    // desaturate color a bit
    float desaturation = 0.3f;
    float darkness = 0.7f;
    //r = r + (1.0f - r) * desaturation * darkness;
    //g = g + (1.0f - g) * desaturation * darkness;
    //b = b + (1.0f - b) * desaturation * darkness;

    // return result
    return XMFLOAT4(r, g, b, 1.0f);
}


Mesh ChunkMeshBuilder::Build(const ChunkKey& chunkKey, Chunk& chunk, ID3D11Device* device)
{
    // setup pointers to chunk and its neighbors
    ChunkCluster localChunks(chunk);

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0)
            {
                // we already have the center chunk
                continue;
            }

            ChunkKey NeighborKey = {chunkKey.X + i, chunkKey.Z + j};
            ChunkPtr Neighbor = GWorld->TryGetChunk(NeighborKey);
            localChunks.SetNeighbor(Neighbor, i, j);
        }
    }

    // setup mesh builders
    //WorldOperations world(GWorld);
    TMeshBuilder<VoxelGameMeshVertex> solidMeshBuilder;
    TMeshBuilder<VoxelGameMeshVertex> waterMeshBuilder;
    BlockMeshBuilder solidBlocksSubMesh(solidMeshBuilder);
    BlockMeshBuilder waterBlocksSubMesh(waterMeshBuilder);

#if 0
    // debug mesh builders
    solidBlocksSubMesh.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    waterBlocksSubMesh.SetColor(0.0f, 0.0f, 1.0f, 1.0f);
    // set position
    solidBlocksSubMesh.SetMeshRelativeBlockOrigin(0.0f, 0.0f, 0.0f);
    waterBlocksSubMesh.SetMeshRelativeBlockOrigin(3.0f, 0.0f, 0.0f);
    // add faces
    solidBlocksSubMesh.AppendPosXFace();
    solidBlocksSubMesh.AppendNegXFace();
    solidBlocksSubMesh.AppendPosYFace();
    solidBlocksSubMesh.AppendNegYFace();
    solidBlocksSubMesh.AppendPosZFace();
    solidBlocksSubMesh.AppendNegZFace();
    
    waterBlocksSubMesh.AppendPosXFace();
    waterBlocksSubMesh.AppendNegXFace();
    waterBlocksSubMesh.AppendPosYFace();
    waterBlocksSubMesh.AppendNegYFace();
    waterBlocksSubMesh.AppendPosZFace();
    waterBlocksSubMesh.AppendNegZFace();

    // return mesh
    MeshAssembler assembler2(device);
    std::vector<MeshBuilder<PosColNormTexVertex>> builders2 = {solidMeshBuilder, waterMeshBuilder};
    Mesh mesh2 = assembler2.AssembleMesh(builders2);
    return mesh2;    
#endif

    // loop through all blocks in the chunk in XZY order (x increments fastest, y increments slowest)
    BlockIndex blockIndex = 0;
    for (int localY = 0; localY < ChunkHeight; ++localY)
    {
        // check if we're exiting (once per y layer)
        if (GIsRequestingExit)
        {
            return Mesh(); // empty mesh
        }
        
        for (int localZ = 0; localZ < ChunkDepth; ++localZ)
        {
            for (int localX = 0; localX < ChunkWidth; ++localX, ++blockIndex)
            {
                // todo: the chunk mesh builder is in need of a rewrite.
                // - you need to know which faces are touching air and which are touching water to determine which faces to include
                // - you need to know how many blocks are touching each face vertex (in front of face)
                // this means you need to query all blocks in the chunk and all blocks touching the chunk

                Block& CurrentBlock = localChunks.GetBlockFromCluster(localX, localY, localZ);
                if (CurrentBlock.TypeIndex == AIR)
                {
                    continue;
                }

                // get block type
                bool IsWaterBlock = CurrentBlock.TypeIndex == WATER;

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
                bool IsFaceTouchingToAirNegX = localChunks.GetBlockFromCluster(localX - 1, localY, localZ) == AIR; // left
                bool IsFaceTouchingToAirNegY = localChunks.GetBlockFromCluster(localX, localY - 1, localZ) == AIR; // down
                bool IsFaceTouchingToAirNegZ = localChunks.GetBlockFromCluster(localX, localY, localZ - 1) == AIR;
                // backward                                        
                bool IsFaceTouchingToAirPosX = localChunks.GetBlockFromCluster(localX + 1, localY, localZ) == AIR; // right 
                bool IsFaceTouchingToAirPosY = localChunks.GetBlockFromCluster(localX, localY + 1, localZ) == AIR; // up
                bool IsFaceTouchingToAirPosZ = localChunks.GetBlockFromCluster(localX, localY, localZ + 1) == AIR;
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
                    bool IsFaceTouchingToWaterNegX = localChunks.GetBlockFromCluster(localX - 1, localY, localZ) == WATER; // left
                    bool IsFaceTouchingToWaterNegY = localChunks.GetBlockFromCluster(localX, localY - 1, localZ) == WATER; // down
                    bool IsFaceTouchingToWaterNegZ = localChunks.GetBlockFromCluster(localX, localY, localZ - 1) == WATER;
                    bool IsFaceTouchingToWaterPosX = localChunks.GetBlockFromCluster(localX + 1, localY, localZ) == WATER; // right
                    bool IsFaceTouchingToWaterPosY = localChunks.GetBlockFromCluster(localX, localY + 1, localZ) == WATER; // up
                    bool IsFaceTouchingToWaterPosZ = localChunks.GetBlockFromCluster(localX, localY, localZ + 1) == WATER;

                    includePosX = IsFaceTouchingToAirPosX || IsFaceTouchingToWaterPosX;
                    includePosY = IsFaceTouchingToAirPosY || IsFaceTouchingToWaterPosY;
                    includePosZ = IsFaceTouchingToAirPosZ || IsFaceTouchingToWaterPosZ;
                    includeNegX = IsFaceTouchingToAirNegX || IsFaceTouchingToWaterNegX;
                    includeNegY = IsFaceTouchingToAirNegY || IsFaceTouchingToWaterNegY;
                    includeNegZ = IsFaceTouchingToAirNegZ || IsFaceTouchingToWaterNegZ;
                }

                BlockMeshBuilder& blockMeshBuilder = IsWaterBlock ? waterBlocksSubMesh : solidBlocksSubMesh;

                // set position and color for this block
                blockMeshBuilder.SetMeshRelativeBlockOrigin((float)localX, (float)localY, (float)localZ);

                // give each face a slightly different ambient shade
                XMFLOAT4 BlockColor = GetBlockColor(CurrentBlock);
                constexpr float TopFaceAmbientShade = 1.0f;
                constexpr float BottomFaceAmbientShade = 0.5f;
                constexpr float EastWestFaceAmbientShade = 0.8f;
                constexpr float NorthSouthFaceAmbientShade = 0.6f;

                // water is slightly transparent
                BlockColor.w = IsWaterBlock ? 0.7f : 1.0f;

                // top face
                blockMeshBuilder.SetColor(BlockColor.x * TopFaceAmbientShade, BlockColor.y * TopFaceAmbientShade,
                                          BlockColor.z * TopFaceAmbientShade, BlockColor.w);
                // for each vertex of the top face, of the four blocks in above and adj to that each verte of the face, how many are touching it (in front of face) are solid (for ao)


                if (includePosY) blockMeshBuilder.AppendPosYFace();

                // bottom face
                blockMeshBuilder.SetColor(BlockColor.x * BottomFaceAmbientShade, BlockColor.y * BottomFaceAmbientShade,
                                          BlockColor.z * BottomFaceAmbientShade, BlockColor.w);
                if (includeNegY) blockMeshBuilder.AppendNegYFace();

                // east/west faces
                blockMeshBuilder.SetColor(BlockColor.x * EastWestFaceAmbientShade,
                                          BlockColor.y * EastWestFaceAmbientShade,
                                          BlockColor.z * EastWestFaceAmbientShade, BlockColor.w);
                if (includePosX) blockMeshBuilder.AppendPosXFace();
                if (includeNegX) blockMeshBuilder.AppendNegXFace();

                // north/south faces
                blockMeshBuilder.SetColor(BlockColor.x * NorthSouthFaceAmbientShade,
                                          BlockColor.y * NorthSouthFaceAmbientShade,
                                          BlockColor.z * NorthSouthFaceAmbientShade, BlockColor.w);
                if (includePosZ) blockMeshBuilder.AppendPosZFace();
                if (includeNegZ) blockMeshBuilder.AppendNegZFace();
            } // end inner block loop
        } // end middle block loop
    } // end outer block loop


    //Mesh mesh = builder.ToMesh();

    // store the chunk key in the mesh for debugging

    auto DebugString = std::make_shared<std::string>(
        "ChunkMesh: " + std::to_string(chunkKey.X) + ", " + std::to_string(chunkKey.Z));


    MeshAssembler assembler(device);
    std::vector<TMeshBuilder<VoxelGameMeshVertex>> builders = {solidMeshBuilder, waterMeshBuilder};
    Mesh mesh = assembler.AssembleMesh(builders, DebugString->c_str());

    mesh.DebugTag = DebugString;

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
