
# VoxelGame

A personal project exploring a standalone C++ game engine using voxels/blocks and procedural world generation.

Uses no libraries other than C++ std and core Direct3D

- Core Defines and Macros
- Platform agnostic classes (timing, OS window, threading)
- Multi-threaded tasks (threadpool)
- Core 3D Math classes (matrix, vector, intvector, plane, frustum)
- 3D Noise generation (perlin, simplex)
- Camera, PlayerController, InputManager
- VoxelCore Library (worlds, chunks, blocks)
- Graphis classes tracking pipeline state and supporting multiple render passes
- HLSL shaders
- Mesh Builder and assembly classes
- Launch / Application classes
- Console, Logging, DebugLines
- Unit tests
- Organized into multiple libraries (.dlls)
- Custom build properties (auto generate .h includes for dll exports)


[video on LinkedIn](https://www.linkedin.com/feed/update/urn:li:activity:7228283928715968512/)

![Chunk Debug Visualization](Images/Chunks.png)
