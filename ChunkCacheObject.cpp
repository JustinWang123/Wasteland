#include "ChunkCacheObject.h"
#include "MapChunkInstance.h"

ChunkCacheObject :: ChunkCacheObject(MapChunkInstance* setChunk, int setCachedTime)
{
	chunk = setChunk;
	cachedTime = setCachedTime;
};

ChunkCacheObject :: ~ChunkCacheObject()
{
	delete chunk;
};