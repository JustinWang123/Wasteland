#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "Perameters.h"
#include "IrrWrapper.h"
#include "MapChunkInstance.h"
#include "ChunkCacheObject.h"
#include <string>
#include <vector>
#include <queue>
#include <map>

class SimpleBlock;
class ComplexBlock;

// This is the number of chunks displayed at one time:
const int NUM_CHUNKS_X = 8;
const int NUM_CHUNKS_Z = 8;

// This is the maximum size of the world:
const int MAP_CHUNK_SIZE_X = 24;
const int MAP_CHUNK_SIZE_Z = 8;

// This is the number of chunks that are stored (cached)
const int CHUNK_CACHE_MAX_SIZE = 50;

class MapManager
{
public:
	static MapManager* Instance();
	~MapManager();
	void Destroy();
    void LoadInitialChunks();

	// Update Methods:
	void Update();

	// File methods:
	void SaveWorld(std::string fileName);
	void LoadWorld(std::string fileName);

	// Map Accessor Methods:
	bool IsBlockSolid(vector3di mapIndex);
	bool IsBlockOpen(vector3di mapIndex);
	int	 GetBlockHP(vector3di mapIndex);
	bool DamageBlock(vector3di mapIndex, int amount);
	void BuildBlock(vector3di mapIndex, int type);
	void BuildLineOfBlocks(int blockID, vector3df start, vector3df dirrection, float distance);
	int  GetChunkInstanceID(vector3df pos);

	// Map Collision Methods:
	bool IsBlockSolid(vector3df start, vector3df dirrection, float distance);
	bool GetLastOpenBlock(vector3df start, vector3df dirrection, float distance, vector3di& openBlock);
	bool GetFirstClosedBlock(vector3df start, vector3df dirrection, float distance, vector3di& closedBlock);
	bool GetPointOfMapCollision(vector3df start, vector3df dirrection, float distance, vector3df& collisionPoint);
	void GetAdjacentVoxelsOrdered(vector3df position, vector3di voxel, std::vector<vector3di>* voxelList);
	bool IsVoxelWalkable(vector3di voxelPos, int voxelHeightOfChar);
	bool IsDiagonalWalkable(vector3di voxelStartPos, vector3di voxelEndPos, int voxelHeightOfChar);

	// Overloaded for floats:
	bool IsBlockSolid(vector3df pos)				{return IsBlockSolid(vector3di(pos.X, pos.Y, pos.Z));}
	bool IsBlockOpen(vector3df pos)					{return IsBlockOpen(vector3di(pos.X, pos.Y, pos.Z));}
	int	GetBlockHP(vector3df pos)					{return GetBlockHP(vector3di(pos.X, pos.Y, pos.Z));}
	void BuildBlock(vector3df pos, int type)		{return BuildBlock(vector3di(pos.X, pos.Y, pos.Z), type);}
	bool DamageBlock(vector3df pos, int damage)		{return DamageBlock(vector3di(pos.X, pos.Y, pos.Z), damage);}

private:
	// Singleton has private constructor:
	MapManager();

	// Internal Updating Methods:
	void UpdateChunksAroundPlayer();
	void UpdateChunkCache();
	void CacheChunk(MapChunkInstance* chunk);
	MapChunkInstance* GetCachedChunk(vector3di position);
	MapChunkInstance* LoadChunk(vector3di chunkPos);

	// Internal Accessors:
	int	GetBlockID(vector3di pos);
	int GetBlockID(vector3df pos)				{return GetBlockID(vector3di(pos.X, pos.Y, pos.Z));}
	bool IsPosInBounds(vector3di pos); // CHANGE NAME
	bool IsChunkInBounds(vector3di pos); // CHANGE NAME
	vector3di GetChunkIndex(vector3di pos);
	vector3di GetInternalChunkPos(vector3di worldPos);

	// Map Chunks:
	vector3di							chunkPosition;
	MapChunkInstance*					MapChunkInstances[NUM_CHUNKS_X][NUM_CHUNKS_Z];
	std::vector<ChunkCacheObject*>		chunkCache;
	std::queue<MapChunkInstance*>		chunkLoadQueue;
};

#define MapMgr MapManager::Instance()
#endif
