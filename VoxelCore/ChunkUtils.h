#pragma once
#include <cstdint>
#include <utility>
#include <vector>

#include "Coordinates.h"
#include "VoxelDefines.h"

#include "Conversions.h"

#undef max // Required for std::min
#undef min // Required for std::max
#include <algorithm>


extern bool GIsRequestingExit;

struct VOXELCORE_API ChunkUtils
{
    /**
     * @brief Checks if a point in 3D space is within a given sphere.
     *
     * This function calculates the squared distance between a point (point_x, point_y, point_z)
     * and the center of a sphere (center_x, center_y, center_z). It then compares this distance
     * to the square of the radius to determine if the point lies inside or on the sphere.
     *
     * @param point_x X-coordinate of the point in 3D space.
     * @param point_y Y-coordinate of the point in 3D space.
     * @param point_z Z-coordinate of the point in 3D space.
     * @param center_x X-coordinate of the center of the sphere.
     * @param center_y Y-coordinate of the center of the sphere.
     * @param center_z Z-coordinate of the center of the sphere.
     * @param radius The radius of the sphere.
     * @return true if the point is within or on the sphere, false otherwise.
     */
    static bool IsPointInSphere(const int point_x, const int point_y, const int point_z, const int center_x,
    const int center_y, const int center_z, const int radius)
    {
        int bias = 1; // Bias to avoid floating point errors
        int distance_squared = (point_x - center_x) * (point_x - center_x) +
            (point_y - center_y) * (point_y - center_y) +
            (point_z - center_z) * (point_z - center_z);

        int radius_squared = radius * radius; 
        return distance_squared <= radius_squared - bias;
    }


    /// @brief Efficiently fills a subvolume within a chunk by applying a given operation to each block.
    /// @tparam T The type of the block data (e.g., uint8_t, uint16_t).
    /// @tparam Func The type of the function or lambda that performs the operation on each block.
    /// @param block_data Pointer to the block data array that represents the chunk.
    /// @param start_index The starting index in the block data array where the subvolume begins.
    /// @param sub_width The width (X dimension) of the subvolume.
    /// @param sub_height The height (Y dimension) of the subvolume.
    /// @param sub_depth The depth (Z dimension) of the subvolume.
    /// @param world_x The starting X coordinate in world space.
    /// @param world_y The starting Y coordinate in world space.
    /// @param world_z The starting Z coordinate in world space.
    /// @param block_operation A function or lambda that is applied to each block in the subvolume. 
    ///        The operation receives the block data pointer, the current block index, 
    ///        and the corresponding world coordinates (x, y, z).
    ///
    /// This function iterates over a subvolume defined by its width, height, and depth within a chunk.
    /// For each block in this subvolume, the provided `block_operation` is invoked, passing the block's data,
    /// its index in the chunk's array, and its corresponding world coordinates.
    ///
    /// The iteration is performed in a YZX order:
    /// - The outer loop iterates over the Y layers (height) of the subvolume.
    /// - The middle loop iterates over the Z rows (depth) within each Y layer.
    /// - The inner loop iterates over the X blocks (width) within each Z row.
    template <typename T, typename Func>
    static void TFillChunkSubvolume(T* const block_data,
                                    int start_index, // starting block index
                                    const int sub_width, const int sub_height, const int sub_depth, 
                                    const int world_x, const int world_y, const int world_z, // starting world block
                                    Func&& block_operation) // Pass the block operation as a template parameter
    {
        // for each y layer in the subvolume
        for (int y = 0; y < sub_height; ++y)
        {
            // check if we're exiting
            if (GIsRequestingExit)
            {
                return;
            }
            
            // for each z row in the current y layer
            int row_index = start_index;
            for (int z = 0; z < sub_depth; ++z)
            {
                // for each x block in the current z row
                int block_index = row_index;
                for (int x = 0; x < sub_width; ++x)
                {
                    // Perform the block operation on the current index using std::forward
                    std::forward<Func>(block_operation)(block_data, block_index, world_x + x, world_y + y, world_z + z);
                    block_index++;
                }
                row_index += ChunkWidth; // advance one full z row (16 blocks)
            }
            start_index += ChunkLayerSize; // advance one full y layer (256 blocks)
        }
    }

    // struct FillSphere_BlockOperation
    // {
    //     template <typename T>
    //     void operator()(T* block_states, int block_index, int world_x, int world_y, int world_z, T block_type)
    //     {
    //         if (IsPointInSphere(world_x, world_y, world_z, 8, 8, 8, 5))
    //         {
    //             block_states[block_index] = block_type;
    //         }
    //     }
    // };

    // static void TestFill()
    // {
    //     uint16_t chunk_block_states[CHUNK_TOTAL_BLOCKS];
    //     uint8_t chunk_light_levels[CHUNK_TOTAL_BLOCKS];
    //
    //     uint16_t block_type = 5;
    //     uint8_t light_level = 12;
    //
    //     //WorldBlockCoord block_world;
    //     //LocalBlockCoord block_local;
    //
    //     FillSphere_BlockOperation fill_sphere_op;
    //
    //     TFillChunkSubvolume(chunk_block_states,
    //                         0, // starting index
    //                         15, 15, 15, // subvolume dimensions
    //                         -100, 200, 2114, // world offset
    //                         [&](auto* block_states, int index, int world_x, int world_y, int world_z)
    //                         {
    //                             fill_sphere_op(block_states, index, world_x, world_y, world_z, block_type);
    //                         });
    //
    //     TFillChunkSubvolume(chunk_light_levels,
    //                         0, // starting index
    //                         4, 5, 6, // subvolume dimensions
    //                         -100, 200, 2114, // world offset
    //                         [&](auto* light_levels, int index, int world_x, int world_y, int world_z)
    //                         {
    //                             light_levels[index] = light_level;
    //                         });
    // }
};

class ChunkVolumeMapper
{
public:

    
    // Static method that returns the list of chunks and their corresponding sub-volume dimensions
    static std::vector<ChunkRegion> GetChunksAndSubVolumes(const BlockRegion& worldRegion)
    {
        std::vector<ChunkRegion> result;

        // Convert the world volume's min and max coordinates to chunk keys
        ChunkKey chunkKeyMin, chunkKeyMax;
        WorldToLocal(worldRegion.Min, chunkKeyMin);
        WorldToLocal(worldRegion.Max, chunkKeyMax);

        // Iterate over each chunk in the range
        for (int cx = chunkKeyMin.X; cx <= chunkKeyMax.X; ++cx)
        {
            for (int cz = chunkKeyMin.Z; cz <= chunkKeyMax.Z; ++cz)
            {
                ChunkKey currentChunkKey = {cx, cz};

                // Convert chunk key back to the world coordinates to determine the chunk origin
                BlockWorldCoordinate chunkOrigin;
                ChunkToWorld(currentChunkKey, chunkOrigin);

                // Calculate the sub-volume starting block and dimensions within the current chunk
                LocalBlockPosition localBlock;
                BlockWorldCoordinate startCoord = worldRegion.Min;
                
                startCoord.X = std::max(worldRegion.Min.X, chunkOrigin.X);
                startCoord.Y = std::max(worldRegion.Min.Y, chunkOrigin.Y);
                startCoord.Z = std::max(worldRegion.Min.Z, chunkOrigin.Z);

                WorldToLocal(startCoord, localBlock);

                int x_min = std::max(worldRegion.Min.X - chunkOrigin.X, 0);
                int x_max = std::min(worldRegion.Max.X - chunkOrigin.X, ChunkWidth - 1);
                int y_min = std::max(worldRegion.Min.Y, 0);
                int y_max = std::min(worldRegion.Max.Y, ChunkHeight - 1);
                int z_min = std::max(worldRegion.Min.Z - chunkOrigin.Z, 0);
                int z_max = std::min(worldRegion.Max.Z - chunkOrigin.Z, ChunkDepth - 1);

                int width = x_max - x_min + 1;
                int height = y_max - y_min + 1;
                int depth = z_max - z_min + 1;

                result.push_back({localBlock, width, height, depth});
            }
        }

        return result;
    }
};
