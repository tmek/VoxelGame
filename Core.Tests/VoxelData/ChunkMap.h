#pragma once

#include <memory>
#include "unordered_map" 

struct ChunkKey;
struct Chunk;
struct ChunkKeyHash;

using ChunkMap = std::unordered_map<ChunkKey, std::shared_ptr<Chunk>, ChunkKeyHash>;
