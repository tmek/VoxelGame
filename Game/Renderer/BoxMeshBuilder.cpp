
#include "VoxelGameMeshVertex.h"
#include "RHI/MeshAssembler.h"
#include "BoxMeshBuilder.h"

Mesh BoxMeshBuilder::Build(ID3D11Device* device)
{
    TMeshBuilder<VoxelGameMeshVertex> meshBuilder;

    // XMFLOAT3 BoxOrigin = {0.0f, 0.0f, 0.0f};
    //
    // XMFLOAT3 frontFaceUVW[] = {{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}};
    // XMFLOAT3 backFaceUVW[] = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}};
    // XMFLOAT3 topFaceUVW[] = {{0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}};
    // XMFLOAT3 bottomFaceUVW[] = {{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}};
    // XMFLOAT3 leftFaceUVW[] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0}};
    // XMFLOAT3 rightFaceUVW[] = {{1, 0, 0}, {1, 0, 1}, {1, 1, 1}, {1, 1, 0}};

    // right face uvw = { 1, 0, 0 }, { 1, 0, 1 }, { 1, 1, 1 }, { 1, 1, 0 }   

    int x = 10;
    int y = 10;
    int z = 10;

    // +z face
    // print face for debugging
    //printf("\n+z face\n");
    BuildFaceFromUVWAxes(-X_AXIS, +Y_AXIS, +Z_AXIS, x, y, z, meshBuilder, {0.0f, 1.0f, 0.0f, 1.0f});

    // -z face
    // print face for debugging
    //printf("-z face\n");
    BuildFaceFromUVWAxes(+X_AXIS, +Y_AXIS, -Z_AXIS, x, y, z, meshBuilder, {1.0f, 0.0f, 0.0f, 0.5f});

    // +x face : U = +Z, V = +Y, W = +X
    // print face for debugging
    //printf("+x face\n");    
    BuildFaceFromUVWAxes(Z_AXIS, Y_AXIS, X_AXIS, x, y, z, meshBuilder, {0.0f, 0.0f, 1.0f, 1.0f});

    // -x face : U = -Z, V = +Y, W = -X
    BuildFaceFromUVWAxes(-Z_AXIS, Y_AXIS, -X_AXIS, x, y, z, meshBuilder, {0.0f, 1.0f, 1.0f, 0.5f});

    // +Y face : U = +X, V = +Z, W = +Y
    BuildFaceFromUVWAxes(-X_AXIS, -Z_AXIS, Y_AXIS, x, y, z, meshBuilder, {1.0f, 0.0f, 1.0f, 1.0f});

    // -Y face : U = +X, V = -Z, W = -Y
    BuildFaceFromUVWAxes(X_AXIS, -Z_AXIS, -Y_AXIS, x, y, z, meshBuilder, {1.0f, 1.0f, 0.0f, 0.5f});

    MeshAssembler assembler(device);
    std::vector<TMeshBuilder<VoxelGameMeshVertex>> meshBuilders = {meshBuilder};
    Mesh mesh = assembler.AssembleMesh(meshBuilders);

    return mesh;
}

void BoxMeshBuilder::BuildFaceFromUVWAxes(const int UAxis, const int VAxis, const int WAxis,
                                          const int localX, const int localY, const int localZ,
                                          TMeshBuilder<VoxelGameMeshVertex>& meshBuilder,
                                          XMFLOAT4 color)
{
    // Assert axes are valid and distinct
    assert(UAxis != 0 && VAxis != 0 && WAxis != 0);
    assert(UAxis != VAxis && VAxis != WAxis && UAxis != WAxis);

    // Determine the indices of the axes
    const int UAxisIndex = abs(UAxis) - 1;
    const int VAxisIndex = abs(VAxis) - 1;
    const int WAxisIndex = abs(WAxis) - 1;

    // Predefined offsets for each vertex in UV space, using a 2D array with floats, in counter-clockwise order 
    static constexpr int UVs[4][2] = {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1}
    };

    // Determine the normal using the WAxis
    XMFLOAT3 normal = {0.0f, 0.0f, 0.0f};
    float* ptr_normal = &normal.x;
    ptr_normal[WAxisIndex] = WAxis > 0 ? 1.0f : -1.0f;

    std::vector<VoxelGameMeshVertex> faceVertices;
    faceVertices.reserve(4);

    for (int vi = 0; vi < 4; ++vi)
    {
        // For each vertex, map the UV coordinates to the appropriate world axes
        XMFLOAT3 position = {static_cast<float>(localX), static_cast<float>(localY), static_cast<float>(localZ)};
        float* pposition = &position.x;

        // Adjust the position based on UAxis, VAxis, WAxis
        pposition[UAxisIndex] += UAxis > 0 ? static_cast<float>(UVs[vi][0]) : 1.0f - static_cast<float>(UVs[vi][0]);
        pposition[VAxisIndex] += VAxis > 0 ? static_cast<float>(UVs[vi][1]) : 1.0f - static_cast<float>(UVs[vi][1]);
        pposition[WAxisIndex] += WAxis > 0 ? 1 : 0;

        // Use UVWs directly for texture coordinates
        XMFLOAT2 texCoord = {static_cast<float>(UVs[vi][0]), static_cast<float>(UVs[vi][1])};

        // calculate AO
        int OccludingBlocks = 0;

        // Calculate AO block positions using UVWs
        int aoBlockU[3] = {localX, localY, localZ};
        int aoBlockV[3] = {localX, localY, localZ};
        int aoBlockUV[3] = {localX, localY, localZ};

        // calculate u and v block coordiate for this vertex 

        // (0,0) vert = 1 in the opposite direction of the UAxis
        // (1,0) vert = 1 in the direction of the UAxis

        // caluclate u and v block coordinate for this vertex
        int UAxisSign = UAxis > 0 ? 1 : -1;
        int VAxisSign = VAxis > 0 ? 1 : -1;
        int WAxisSign = WAxis > 0 ? 1 : -1;

        int w = WAxisSign;
        aoBlockU[WAxisIndex] += w;
        aoBlockV[WAxisIndex] += w;
        aoBlockUV[WAxisIndex] += w;

        // If U (or V) is 1 we want the block in the direction of the axis,
        // if 0 we want the block in the opposite direction
        int u = UVs[vi][0] ? UAxisSign : -UAxisSign;
        int v = UVs[vi][1] ? VAxisSign : -VAxisSign;
        
        // AO Block along the U axis
        aoBlockU[UAxisIndex] += u;

        // AO Block along the V axis
        aoBlockV[VAxisIndex] += v;

        // AO Block along both U and V axes
        aoBlockUV[UAxisIndex] += u;
        aoBlockUV[VAxisIndex] += v;

        // printf("\nVertex %d: %f, %f, %f\n", vi, position.x, position.y, position.z);
        // printf("Normal: %f, %f, %f\n", normal.x, normal.y, normal.z);
        // printf("UV %d: %f, %f\n", vi, texCoord.x, texCoord.y);
        // printf("Block 1: %d, %d, %d\n", aoBlockU[0], aoBlockU[1], aoBlockU[2]);
        // printf("Block 2: %d, %d, %d\n", aoBlockV[0], aoBlockV[1], aoBlockV[2]);
        // printf("Block 3: %d, %d, %d\n", aoBlockUV[0], aoBlockUV[1], aoBlockUV[2]);
        // fflush(stdout);

        // BlockType ao1BlockType = GetBlockType(aoBlock1[0], aoBlock1[1], aoBlock1[2]);
        // BlockType ao2BlockType = GetBlockType(aoBlock2[0], aoBlock2[1], aoBlock2[2]);
        // BlockType ao3BlockType = GetBlockType(aoBlock3[0], aoBlock3[1], aoBlock3[2]);
        //
        // if (ao1BlockType != AIR && ao1BlockType != WATER) OccludingBlocks++;
        // if (ao2BlockType != AIR && ao2BlockType != WATER) OccludingBlocks++;
        // if (ao3BlockType != AIR && ao3BlockType != WATER) OccludingBlocks++;

        float AOFactor = 1.0f - static_cast<float>(OccludingBlocks) / 4.0f; // 0 blocks = full bright

        // modify color by AO
        XMFLOAT4 aoColor = {color.x * AOFactor, color.y * AOFactor, color.z * AOFactor, color.w};

        // Add the vertex to the faceVertices vector
        faceVertices.push_back({position, aoColor, normal, texCoord});
    }

    // Append the face vertices to the meshBuilder
    std::vector<uint32_t> faceIndices = {0, 1, 2, 0, 2, 3};

    meshBuilder.Append(faceVertices, faceIndices);
}
