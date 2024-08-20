// copyright

#pragma once

#include <cstring> // For memcpy
#include "Chunk/Chunk.h"

#include "WorldGen/BlockTypes.h"

// constexpr int ChunkWidth = 16;
// constexpr int ChunkHeight = 384;
// constexpr int ChunkDepth = 16;
// constexpr int ChunkSize = ChunkWidth * ChunkHeight * ChunkDepth;
//
//
// struct Block
// {
//     int Type; // Example block property
// };
//
// struct Chunk
// {
//     Block Blocks[ChunkSize];
// };
//
// const Block EmptyBlock = {0}; // Default block for empty or missing chunks

struct ChunkClusterProcessor
{
    static constexpr int SpanSize = 18;
    static constexpr int NumLayersY = 3;
    static constexpr int LayerSize = SpanSize * SpanSize;

    Block* SlidingBlocks[NumLayersY][SpanSize][SpanSize];

    void Initialize(Chunk* Chunks[3][3])
    {
        for (int z = 0; z < SpanSize; ++z)
        {
            for (int x = 0; x < SpanSize; ++x)
            {
                // Determine which chunk's blocks to use for the bottom layer
                Chunk* chunk = nullptr;

                if (z == 0)
                {
                    if (x == 0)
                    {
                        chunk = Chunks[0][0]; // Bottom-left-front neighbor
                    }
                    else if (x == SpanSize - 1)
                    {
                        chunk = Chunks[0][2]; // Bottom-right-front neighbor
                    }
                    else
                    {
                        chunk = Chunks[0][1]; // Bottom-front neighbor
                    }
                }
                else if (z == SpanSize - 1)
                {
                    if (x == 0)
                    {
                        chunk = Chunks[2][0]; // Bottom-left-back neighbor
                    }
                    else if (x == SpanSize - 1)
                    {
                        chunk = Chunks[2][2]; // Bottom-right-back neighbor
                    }
                    else
                    {
                        chunk = Chunks[2][1]; // Bottom-back neighbor
                    }
                }
                else
                {
                    if (x == 0)
                    {
                        chunk = Chunks[1][0]; // Bottom-left neighbor
                    }
                    else if (x == SpanSize - 1)
                    {
                        chunk = Chunks[1][2]; // Bottom-right neighbor
                    }
                    else
                    {
                        chunk = Chunks[1][1]; // Center chunk
                    }
                }

                SlidingBlocks[0][z][x] = chunk != nullptr
                                             ? &chunk->Blocks[z * SpanSize + x]
                                             : const_cast<Block*>(&EmptyBlock);

                // Initialize the center and top layers
                Chunk* centerChunk = Chunks[1][1];
                if (z == 0 || z == SpanSize - 1 || x == 0 || x == SpanSize - 1)
                {
                    // Handle edge cases for center and top layers using neighboring chunks
                    SlidingBlocks[1][z][x] = chunk != nullptr
                                                 ? &chunk->Blocks[(ChunkHeight / NumLayersY) * 0 + z * SpanSize + x]
                                                 : const_cast<Block*>(&EmptyBlock);
                    SlidingBlocks[2][z][x] = chunk != nullptr
                                                 ? &chunk->Blocks[(ChunkHeight / NumLayersY) * 1 + z * SpanSize + x]
                                                 : const_cast<Block*>(&EmptyBlock);
                }
                else
                {
                    // Non-edge blocks from center chunk
                    SlidingBlocks[1][z][x] = centerChunk
                                                 ? &centerChunk->Blocks[0 * LayerSize + z * SpanSize + x]
                                                 : const_cast<Block*>(&EmptyBlock);
                    SlidingBlocks[2][z][x] = centerChunk
                                                 ? &centerChunk->Blocks[1 * LayerSize + z * SpanSize + x]
                                                 : const_cast<Block*>(&EmptyBlock);
                }
            }
        }
    }


    void ProcessCenterBlocks(Chunk* Chunks[3][3])
    {
        Initialize(Chunks);

        int offset = 0;
        for (int ChunkY = 0; ChunkY < ChunkHeight; ++ChunkY)
        {
            int ShiftLayerStart = 0;
            int ShiftLayerEnd = NumLayersY * LayerSize;

            if (ChunkY == 1)
            {
                // first edge case happens at second layer of chunk
                std::memcpy(SlidingBlocks[0], SlidingBlocks[1], sizeof(Block*) * LayerSize);
                // Copy the center layer to the bottom layer to replace the EmptyBlocks.
                ShiftLayerStart = LayerSize;
            }
            else if (ChunkY == ChunkHeight - 1)
            {
                // second edge case happens at top layer of chunk
                for (int i = 0; i < LayerSize; ++i)
                {
                    SlidingBlocks[2][0][i] = const_cast<Block*>(&EmptyBlock);
                }
                ShiftLayerEnd = 2 * LayerSize;
            }

            // shift the layers upwards
            for (int i = ShiftLayerStart; i < ShiftLayerEnd; ++i)
            {
                reinterpret_cast<Block**>(SlidingBlocks)[i] += LayerSize;
            }

            // process the layer blocks
            for (int z = 0; z < SpanSize; ++z)
            {
                for (int x = 0; x < SpanSize; ++x)
                {
                    // middle blocks
                    Block* centerBlockPtr = SlidingBlocks[1][1][1] + offset;
                    Block* blockFrontPtr = SlidingBlocks[1][0][1] + offset;
                    Block* blockBackPtr = SlidingBlocks[1][2][1] + offset;
                    Block* blockLeftPtr = SlidingBlocks[1][1][0] + offset;
                    Block* blockRightPtr = SlidingBlocks[1][1][2] + offset;
                    Block* blockFrontLeftPtr = SlidingBlocks[1][0][0] + offset;
                    Block* blockFrontRightPtr = SlidingBlocks[1][0][2] + offset;
                    Block* blockBackLeftPtr = SlidingBlocks[1][2][0] + offset;
                    Block* blockBackRightPtr = SlidingBlocks[1][2][2] + offset;

                    // above blocks
                    Block* blockAbovePtr = SlidingBlocks[2][1][1] + offset;
                    Block* blockAboveFrontPtr = SlidingBlocks[2][0][0] + offset;
                    Block* blockAboveBackPtr = SlidingBlocks[2][0][2] + offset;
                    Block* blockAboveLeftPtr = SlidingBlocks[2][0][1] + offset;
                    Block* blockAboveRightPtr = SlidingBlocks[2][2][1] + offset;
                    Block* blockAboveFrontLeftPtr = SlidingBlocks[2][0][0] + offset;
                    Block* blockAboveFrontRightPtr = SlidingBlocks[2][0][2] + offset;
                    Block* blockAboveBackLeftPtr = SlidingBlocks[2][2][0] + offset;
                    Block* blockAboveBackRightPtr = SlidingBlocks[2][2][2] + offset;

                    // below blocks
                    Block* blockBelowPtr = SlidingBlocks[0][1][1] + offset;
                    Block* blockBelowFrontPtr = SlidingBlocks[0][0][0] + offset;
                    Block* blockBelowBackPtr = SlidingBlocks[0][0][2] + offset;
                    Block* blockBelowLeftPtr = SlidingBlocks[0][0][1] + offset;
                    Block* blockBelowRightPtr = SlidingBlocks[0][2][1] + offset;
                    Block* blockBelowFrontLeftPtr = SlidingBlocks[0][0][0] + offset;
                    Block* blockBelowFrontRightPtr = SlidingBlocks[0][0][2] + offset;
                    Block* blockBelowBackLeftPtr = SlidingBlocks[0][2][0] + offset;
                    Block* blockBelowBackRightPtr = SlidingBlocks[0][2][2] + offset;

                    {
                        
                        if (blockFrontPtr->Type == AIR)
                        {
                            // include front face.
                             

                            // calculate blocks contributing to ambient occlusion for front face verts
                            int vertFrontRightTopAOC = blockFrontRightPtr->Type == AIR ? 1 : 0 + blockAboveFrontRightPtr->Type == AIR ? 1 : 0 + blockAboveFrontPtr->Type == AIR ? 1 : 0;
                            int vertFrontRightBottomAOC = blockFrontRightPtr->Type == AIR ? 1 : 0 + blockBelowFrontRightPtr->Type == AIR ? 1 : 0 + blockBelowFrontPtr->Type == AIR ? 1 : 0;
                            int vertFrontLeftTopAOC = blockFrontLeftPtr->Type == AIR ? 1 : 0 + blockAboveFrontLeftPtr->Type == AIR ? 1 : 0 + blockAboveFrontPtr->Type == AIR ? 1 : 0;
                            int vertFrontLeftBottomAOC = blockFrontLeftPtr->Type == AIR ? 1 : 0 + blockBelowFrontLeftPtr->Type == AIR ? 1 : 0 + blockBelowFrontPtr->Type == AIR ? 1 : 0;
                        }
                    }

                    offset++;
                }
                offset += 2;
            }
        }
    }
};
