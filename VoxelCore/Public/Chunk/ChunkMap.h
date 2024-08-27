// Copyright

#pragma once

#include <memory>
#include "Chunk.h"
#include "ChunkKey.h"
#include "ChunkKeyHash.h"
#include "unordered_map" 


/** Associative container for chunks. */ 
using ChunkMap = std::unordered_map<ChunkKey, ChunkRef, ChunkKeyHash>;
