#pragma once

#include "Block.h"
#include "ChunkConstants.h"

struct Chunk
{
    Block Blocks[ChunkSize];
};