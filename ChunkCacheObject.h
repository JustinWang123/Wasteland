#ifndef CHUNK_CACHE_OBJECT_H
#define CHUNK_CACHE_OBJECT_H

class MapChunkInstance;

struct ChunkCacheObject
{
	ChunkCacheObject(MapChunkInstance* setChunk, int setCachedTime);
	~ChunkCacheObject();

	MapChunkInstance* chunk;
	int cachedTime;
};

#endif