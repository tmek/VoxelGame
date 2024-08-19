// Copyright

#pragma once

#include "Chunk.h"
#include "ChunkKey.h"
#include "ChunkKeyHash.h"
#include "unordered_map" 
#include <memory>


/** Associative container for chunks. */ 
using ChunkMap = std::unordered_map<ChunkKey, ChunkRef, ChunkKeyHash>;
