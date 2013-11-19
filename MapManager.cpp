#include "MapManager.h"
#include "GameManager.h"
#include "PathPlanner.h"
#include "SimpleBlock.h"
#include "BlockManager.h"
#include "InstanceManager.h"
#include <iostream>

// --------------------------------------------------------------------
// Constructor, Destructor, Update:
// --------------------------------------------------------------------
MapManager* MapManager :: Instance()
{
	static MapManager instance;
	return &instance;
}

MapManager :: MapManager ()
{
    chunkPosition = vector3di(96,0,0);
}

void MapManager :: LoadInitialChunks()
{
    std::cout << "MapManager: Loading Initial Chunks\n";

	// Load initial chunks:
	for(int x = 0; x < NUM_CHUNKS_X; x++)
		for(int z = 0; z < NUM_CHUNKS_Z; z++)
		{
			MapChunkInstances[x][z] = new MapChunkInstance( chunkPosition + vector3di(x * CHUNK_SIZE, 0, z * CHUNK_SIZE));
			MapChunkInstances[x][z]->LoadChunkAsOcTree();
		}

    std::cout << "MapManager: Initililizing Chunks\n";

	// Initiate chunks:
	for(int x = 0; x < NUM_CHUNKS_X; x++)
		for(int z = 0; z < NUM_CHUNKS_Z; z++)
		{
			MapChunkInstances[x][z]->InitSceneNode();
			MapChunkInstances[x][z]->Activate();
		}

    std::cout << "MapManager: Done Initilization\n";
}

MapManager :: ~MapManager()
{

}

void MapManager :: Destroy()
{
	for(int x = 0; x < NUM_CHUNKS_X; x++)
		for(int z = 0; z < NUM_CHUNKS_Z; z++)
			delete MapChunkInstances[x][z];

	// flush chunk cache:
	for(std::vector<ChunkCacheObject*>::iterator it = chunkCache.begin(); it != chunkCache.end(); it++)
	{
		delete (*it);
	}

	chunkCache.clear();
}

void MapManager :: Update()
{
	//UpdateChunksAroundPlayer();
	//UpdateChunkCache();
}

// --------------------------------------------------------------------
// Chunk Updating:
// --------------------------------------------------------------------
void MapManager :: UpdateChunksAroundPlayer()
{
/*
	if(InstanceMgr->GetPlayerInstance()->GetFeetPosition().X > chunkPosition.X + ((float)NUM_CHUNKS_X / 1.5f) * CHUNK_SIZE && chunkPosition.X + NUM_CHUNKS_X * CHUNK_SIZE < MAP_CHUNK_SIZE_X * CHUNK_SIZE)
	{
		chunkPosition.X += CHUNK_SIZE;

		for(int z = 0; z < NUM_CHUNKS_Z; z++)
		{
			CacheChunk(MapChunkInstances[0][z]);

			for(int x = 0; x < NUM_CHUNKS_X - 1; x++)
				MapChunkInstances[x][z] = MapChunkInstances[x + 1][z];

			vector3di chunkPos((int)chunkPosition.X + (NUM_CHUNKS_X - 1) * CHUNK_SIZE, 0, (int)chunkPosition.Z + z * CHUNK_SIZE);
			MapChunkInstance* chunk = GetCachedChunk(chunkPos);

			// If the chunk is in cache then get it from there
			if(chunk != NULL)
			{
				MapChunkInstances[NUM_CHUNKS_X - 1][z] = chunk;
				chunk->Activate();
			}
			// Else load/generate a new one
			else
			{
				MapChunkInstances[NUM_CHUNKS_X - 1][z] = LoadChunk(chunkPos);
			}
		}
	}

	if(InstanceMgr->GetPlayerInstance()->GetFeetPosition().X < chunkPosition.X  + ((float)NUM_CHUNKS_X / 3.0f) * CHUNK_SIZE && chunkPosition.X > 0)
	{
		chunkPosition.X -= CHUNK_SIZE;

		for(int z = 0; z < NUM_CHUNKS_Z; z++)
		{
			CacheChunk(MapChunkInstances[NUM_CHUNKS_X - 1][z]);

			for(int x = NUM_CHUNKS_X - 1; x > 0 ; x--)
				MapChunkInstances[x][z] = MapChunkInstances[x - 1][z];

			vector3di chunkPos((int)chunkPosition.X, 0, (int)chunkPosition.Z + z * CHUNK_SIZE);
			MapChunkInstance* chunk = GetCachedChunk(chunkPos);

			if(chunk != 0)
			{
				MapChunkInstances[0][z] = chunk;
				chunk->Activate();
			}
			else
			{
				MapChunkInstances[0][z] = LoadChunk(chunkPos);
			}
		}
	}

	if(InstanceMgr->GetPlayerInstance()->GetFeetPosition().Z > chunkPosition.Z + ((float)NUM_CHUNKS_Z / 1.5f) * CHUNK_SIZE && chunkPosition.Z  + NUM_CHUNKS_Z * CHUNK_SIZE < MAP_CHUNK_SIZE_Z * CHUNK_SIZE)
	{
		chunkPosition.Z += CHUNK_SIZE;

		for(int x = 0; x < NUM_CHUNKS_X; x++)
		{
			CacheChunk(MapChunkInstances[x][0]);

			for(int z = 0; z < NUM_CHUNKS_Z - 1; z++)
				MapChunkInstances[x][z] = MapChunkInstances[x][z + 1];

			vector3di chunkPos((int)chunkPosition.X + x * CHUNK_SIZE, 0, (int)chunkPosition.Z + (NUM_CHUNKS_Z - 1) * CHUNK_SIZE);
			MapChunkInstance* chunk = GetCachedChunk(chunkPos);

			if(chunk != 0)
			{
				MapChunkInstances[x][NUM_CHUNKS_Z - 1] = chunk;
				chunk->Activate();
			}
			else
			{
				MapChunkInstances[x][NUM_CHUNKS_Z - 1] = LoadChunk(chunkPos);
			}
		}
	}

	if(InstanceMgr->GetPlayerInstance()->GetFeetPosition().Z < chunkPosition.Z + ((float)NUM_CHUNKS_Z / 3.0f) * CHUNK_SIZE && chunkPosition.Z > 0)
	{
		chunkPosition.Z -= CHUNK_SIZE;

		for(int x = 0; x < NUM_CHUNKS_X; x++)
		{
			// take the chunks furthest from the player (in the z-dirrection) and place them in the cache
			CacheChunk(MapChunkInstances[x][NUM_CHUNKS_Z - 1]);

			// move all chunks over a space in MapChunkInstances array (in the z-dirrection)
			for(int z = NUM_CHUNKS_Z - 1; z > 0 ; z--)
				MapChunkInstances[x][z] = MapChunkInstances[x][z - 1];

			// check to see if the chunk we are about to load is already in the cache:
			vector3di chunkPos((int)chunkPosition.X + x * CHUNK_SIZE, 0, (int)chunkPosition.Z);
			MapChunkInstance* chunk = GetCachedChunk(chunkPos);

			// If it is then simply take it from the cache and activate it:
			if(chunk != 0)
			{
				MapChunkInstances[x][0] = chunk;
				chunk->Activate();
			}
			// Else we need to load up a new chunk:
			else
			{
				MapChunkInstances[x][0] = LoadChunk(chunkPos);
			}
		}
	}

	if(!chunkLoadQueue.empty())
	{
		chunkLoadQueue.front()->LoadChunkAsOcTree();
		chunkLoadQueue.front()->InitSceneNode();
		chunkLoadQueue.front()->Activate();
		chunkLoadQueue.pop();
	}
	*/
}

void MapManager :: UpdateChunkCache()
{
/*
	if(chunkCache.size() > CHUNK_CACHE_MAX_SIZE)
	{
		std::vector<ChunkCacheObject*>::iterator oldestChunk = chunkCache.begin();

		for(std::vector<ChunkCacheObject*>::iterator it = chunkCache.begin(); it != chunkCache.end(); it++)
		{
			if((*it)->cachedTime < (*oldestChunk)->cachedTime)
			{
				oldestChunk = it;
			}
		}

		delete (*oldestChunk);
		chunkCache.erase(oldestChunk);
	}
	*/
}

void MapManager :: CacheChunk(MapChunkInstance* chunk)
{
	chunk->Deactivate();
	chunkCache.push_back(new ChunkCacheObject(chunk, IRR->GetTime()));
}

MapChunkInstance*	MapManager :: GetCachedChunk(vector3di position)
{
	for(std::vector<ChunkCacheObject*>::iterator it = chunkCache.begin(); it != chunkCache.end(); it++)
		if((*it)->chunk->GetPositionInt() == position)
		{
			MapChunkInstance* chunk = (*it)->chunk;
			chunkCache.erase(it);
			return chunk;
		}

	return 0;
}

MapChunkInstance* MapManager :: LoadChunk(vector3di chunkPos)
{
	MapChunkInstance* instance = new MapChunkInstance(chunkPos);
	chunkLoadQueue.push(instance);
	return instance;
}

// --------------------------------------------------------------------
// Save and Load World:
// --------------------------------------------------------------------
void MapManager :: SaveWorld(std::string fileName)
{
	/*
	ofstream myfile; // Creat output file:
	myfile.open (fileName.c_str()); // create or overwrite file:


	for(int x = 0; x < MAP_SIZE_X; x++)
		for(int y = 0; y < MAP_SIZE_Y; y++)
			for(int z = 0; z < MAP_SIZE_Z; z++)
			{
				myfile << mapData[x][y][z] << " ";
				//myfile << mapHealth[x][y][z] << " ";
			}


	myfile << itemInstances.size() << " ";

	for(std::vector<ItemInstance*>::iterator it = itemInstances.begin(); it != itemInstances.end(); it++)
	{
		myfile << (*it)->GetItemID() << " ";
		myfile << (*it)->GetSceneNode()->getPosition().X << " ";
		myfile << (*it)->GetSceneNode()->getPosition().Y << " ";
		myfile << (*it)->GetSceneNode()->getPosition().Z << " ";
		myfile << (*it)->GetSceneNode()->getRotation().X << " ";
		myfile << (*it)->GetSceneNode()->getRotation().Y << " ";
		myfile << (*it)->GetSceneNode()->getRotation().Z << " ";
	}

	myfile.close();
	*/
}

void MapManager :: LoadWorld(std::string fileName)
{
	/*
	ifstream myfile; // Creat output file:
	myfile.open (fileName.c_str()); // create or overwrite file:


	for(int x = 0; x < MAP_SIZE_X; x++)
		for(int y = 0; y < MAP_SIZE_Y; y++)
			for(int z = 0; z < MAP_SIZE_Z; z++)
			{
				myfile >> mapData[x][y][z];
				//myfile >> mapHealth[x][y][z];

			}

	int numOfItems;
	int itemID;
	vector3df itemPos;
	vector3df itemRot;

	myfile >> numOfItems;

	for(int i = 0; i < numOfItems; i++)
	{
		myfile >> itemID;
		myfile >> itemPos.X;
		myfile >> itemPos.Y;
		myfile >> itemPos.Z;
		myfile >> itemRot.X;
		myfile >> itemRot.Y;
		myfile >> itemRot.Z;

		GManager->AddItem(itemID, itemPos, itemRot);
	}

	myfile.close();

	UpdateAllChunks();
	*/
}

// --------------------------------------------------------------------
// Map Accessor Methods:
// --------------------------------------------------------------------
bool MapManager :: IsPosInBounds(vector3di pos)
{
	return pos.X >= 0
		&& pos.Y >= 0
		&& pos.Z >= 0
		&& pos.X < MAP_CHUNK_SIZE_X * CHUNK_SIZE
		&& pos.Y < CHUNK_HEIGHT
		&& pos.Z < MAP_CHUNK_SIZE_Z * CHUNK_SIZE;
}

bool MapManager :: IsChunkInBounds(vector3di pos)
{
	return pos.X >= 0
		&& pos.Z >= 0
		&& pos.X < NUM_CHUNKS_X
		&& pos.Z < NUM_CHUNKS_Z;
}

bool MapManager :: IsBlockSolid(vector3di pos)
{
	if(IsPosInBounds(pos) && IsChunkInBounds(GetChunkIndex(pos)))
	{
		return MapChunkInstances[GetChunkIndex(pos).X][GetChunkIndex(pos).Z]->IsBlockSolid(GetInternalChunkPos(pos));
	}
	else
	{
		return false; // Out of bound blocks are considered solid
	}
}

bool MapManager :: IsBlockOpen(vector3di pos)
{
	return !IsBlockSolid(pos);
}

int MapManager :: GetBlockID(vector3di pos)
{
	if(IsPosInBounds(pos) && IsChunkInBounds(GetChunkIndex(pos)))
	{
		return MapChunkInstances[GetChunkIndex(pos).X][GetChunkIndex(pos).Z]->GetBlockID(GetInternalChunkPos(pos));
	}

	return -1;
}

vector3di MapManager :: GetChunkIndex(vector3di pos)
{
	int chunkX = (pos.X - chunkPosition.X) / CHUNK_SIZE;
	int chunkY = 0;
	int chunkZ = (pos.Z - chunkPosition.Z) / CHUNK_SIZE;

	return vector3di(chunkX, chunkY, chunkZ);
}

vector3di MapManager :: GetInternalChunkPos(vector3di worldPos)
{
	int x = (worldPos.X - chunkPosition.X) % CHUNK_SIZE;
	int y = worldPos.Y;
	int z = (worldPos.Z - chunkPosition.Z) % CHUNK_SIZE;

	return vector3di(x,y,z);
}

int	MapManager :: GetBlockHP(vector3di pos)
{
	if(IsPosInBounds(pos) && IsChunkInBounds(GetChunkIndex(pos)))
	{
		return MapChunkInstances[GetChunkIndex(pos).X][GetChunkIndex(pos).Z]->GetBlockHP(GetInternalChunkPos(pos));
	}
	else
	{
		return -1;
	}
}

void MapManager :: BuildBlock(vector3di pos, int type)
{
	if(IsPosInBounds(pos) && IsChunkInBounds(GetChunkIndex(pos)))
	{
		MapChunkInstances[GetChunkIndex(pos).X][GetChunkIndex(pos).Z]->BuildBlock(GetInternalChunkPos(pos), type);
	}
}

bool MapManager :: DamageBlock(vector3di pos, int amount)
{
	if(IsPosInBounds(pos) && IsChunkInBounds(GetChunkIndex(pos)))
	{
		return MapChunkInstances[GetChunkIndex(pos).X][GetChunkIndex(pos).Z]->DamageBlock(GetInternalChunkPos(pos), amount);
	}

	return false;
}

int MapManager :: GetChunkInstanceID(vector3df pos)
{
	vector3di chunkIndex = GetChunkIndex(vector3di(pos.X, pos.Y, pos.Z));

	if(chunkIndex.X > 0 && chunkIndex.X < NUM_CHUNKS_X
	&& chunkIndex.Z > 0 && chunkIndex.Z < NUM_CHUNKS_Z)
	{
		return MapChunkInstances[chunkIndex.X][chunkIndex.Z]->GetID();
	}
	else
	{
		return -1;
	}
}


// --------------------------------------------------------------------
// Collision Methods:
// --------------------------------------------------------------------
bool MapManager :: IsBlockSolid(vector3df start, vector3df dirrection, float distance)
{
	vector3df currentPos = start;
	dirrection.normalize();
	float stepSize = 0.25f;

	for(float currentLength = 0.0f; currentLength < distance; currentLength += stepSize)
	{
		currentPos += dirrection * stepSize;

		if(IsBlockSolid(currentPos))
		{
			return true;
		}
	}

	return false;
}

void MapManager :: BuildLineOfBlocks(int blockID, vector3df start, vector3df dirrection, float distance)
{
	vector3df currentPos = start;
	dirrection.normalize();
	float stepSize = 0.25f;
	vector3di lastBlock;

	if(GetLastOpenBlock(start, dirrection, distance, lastBlock))
	{

		for(float currentLength = 0.0f; currentLength < distance; currentLength += stepSize)
		{
			currentPos += dirrection * stepSize;

			if(vector3di(currentPos.X, currentPos.Y, currentPos.Z) != lastBlock)
			{
				BuildBlock(currentPos, blockID);
			}
			else
			{
				BuildBlock(currentPos, blockID);
				break;
			}
		}
	}
}

void MapManager :: GetAdjacentVoxelsOrdered(vector3df position, vector3di voxel, std::vector<vector3di>* voxelList)
{
	std::vector<vector3di> unsortedVoxelList;

	// All on same y level as voxel:
	if( IsBlockOpen(voxel + vector3di(-1, 0, 1)))
	{
		unsortedVoxelList.push_back(voxel + vector3di(-1, 0, 1));
	}

	if( IsBlockOpen(voxel + vector3di(0, 0, 1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(0, 0, 1));
	}

	if( IsBlockOpen(voxel + vector3di(1, 0, 1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(1, 0, 1));
	}

	if( IsBlockOpen(voxel + vector3di(-1, 0, 0) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(-1, 0, 0));
	}

	if( IsBlockOpen(voxel + vector3di(1, 0, 0) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(1, 0, 0));
	}

	if( IsBlockOpen(voxel + vector3di(-1, 0, -1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(-1, 0, -1));
	}

	if( IsBlockOpen(voxel + vector3di(0, 0, -1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(0, 0, -1));
	}

	if( IsBlockOpen(voxel + vector3di(1, 0, -1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(1, 0, -1));
	}

	// All on +1 y level as voxel:
	if( IsBlockOpen(voxel + vector3di(-1, 1, 1)))
	{
		unsortedVoxelList.push_back(voxel + vector3di(-1, 1, 1));
	}

	if( IsBlockOpen(voxel + vector3di(0, 1, 1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(0, 1, 1));
	}

	if( IsBlockOpen(voxel + vector3di(1, 1, 1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(1,1, 1));
	}

	if( IsBlockOpen(voxel + vector3di(-1, 1, 0) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(-1, 1, 0));
	}

	if( IsBlockOpen(voxel + vector3di(1, 1, 0) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(1, 1, 0));
	}

	if( IsBlockOpen(voxel + vector3di(-1, 1, -1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(-1, 1, -1));
	}

	if( IsBlockOpen(voxel + vector3di(0, 1, -1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(0, 1, -1));
	}

	if( IsBlockOpen(voxel + vector3di(1, 1, -1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(1, 1, -1));
	}

	// All on -1 y level as voxel:
	if( IsBlockOpen(voxel + vector3di(-1, -1, 1)))
	{
		unsortedVoxelList.push_back(voxel + vector3di(-1, -1, 1));
	}

	if( IsBlockOpen(voxel + vector3di(0, -1, 1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(0, -1, 1));
	}

	if( IsBlockOpen(voxel + vector3di(1, -1, 1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(1, -1, 1));
	}

	if( IsBlockOpen(voxel + vector3di(-1, -1, 0) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(-1, -1, 0));
	}

	if( IsBlockOpen(voxel + vector3di(1, -1, 0) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(1, -1, 0));
	}

	if( IsBlockOpen(voxel + vector3di(-1, -1, -1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(-1, -1, -1));
	}

	if( IsBlockOpen(voxel + vector3di(0, -1, -1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(0, 1, -1));
	}

	if( IsBlockOpen(voxel + vector3di(1, -1, -1) ))
	{
		unsortedVoxelList.push_back(voxel + vector3di(1, -1, -1));
	}


	while(!unsortedVoxelList.empty())
	{
		vector3di currentVoxel = unsortedVoxelList.back();
		unsortedVoxelList.pop_back();

		bool hasInserted = false;

		// iterate until an element of voxelList is found that has a smaller H value then the currentVoxel
		// insert the current voxel before this element
		for(std::vector<vector3di>::iterator it = voxelList->begin(); it != voxelList->end(); it++)
			if(PPlanner->CalculateH(vector3di(position.X, position.Y, position.Z), currentVoxel) < PPlanner->CalculateH(vector3di(position.X, position.Y, position.Z), (*it)))
			{
				// Insert before it
				voxelList->insert(it, currentVoxel);

				hasInserted = true;
				break;
			}

		// If the currentVoxel still has not been inserted then push it to the back of voxelList
		if(!hasInserted)
		{
			voxelList->push_back(currentVoxel);
		}
	}
}

bool MapManager :: IsVoxelWalkable(vector3di voxelPos, int voxelHeightOfChar)
{
	// Check if the player can 'fit':
	for(int i = 0; i < voxelHeightOfChar; i++)
	{
		// If there is a closed block in the way return 0 (fail)
		if(IsBlockSolid( voxelPos + vector3di(0,i,0) ))
		{
			return 0;
		}
	}

	// Only if there is a closed block within the fall distance will the character walk there:
	if(IsBlockOpen( voxelPos + vector3di(0,-1,0) ))
	{
		return 0;
	}


	return true;
}

bool MapManager :: IsDiagonalWalkable(vector3di voxelStartPos, vector3di voxelEndPos, int voxelHeightOfChar)
{
	bool isWalkable = true;

	vector3di offset = voxelEndPos - voxelStartPos;
	offset.Y = 0;

	if(offset == vector3di(1,0,1))
	{
		if(!IsVoxelWalkable(voxelStartPos + vector3di(1,0,0), voxelHeightOfChar) || !IsVoxelWalkable(voxelStartPos + vector3di(0,0,1), voxelHeightOfChar))
		{
			isWalkable = false;
		}
	}
	else if(offset == vector3di(-1,0,1))
	{
		if(!IsVoxelWalkable(voxelStartPos + vector3di(-1,0,0), voxelHeightOfChar) || !IsVoxelWalkable(voxelStartPos + vector3di(0,0,1), voxelHeightOfChar))
		{
			isWalkable = false;
		}
	}
	else if(offset == vector3di(1,0,-1))
	{
		if(!IsVoxelWalkable(voxelStartPos + vector3di(1,0,0), voxelHeightOfChar) || !IsVoxelWalkable(voxelStartPos + vector3di(0,0,-1), voxelHeightOfChar))
		{
			isWalkable = false;
		}
	}
	else if(offset == vector3di(-1,0,-1))
	{
		if(!IsVoxelWalkable(voxelStartPos + vector3di(-1,0,0), voxelHeightOfChar) || !IsVoxelWalkable(voxelStartPos + vector3di(0,0,-1), voxelHeightOfChar))
		{
			isWalkable = false;
		}
	}

	return isWalkable;
}

bool MapManager :: GetLastOpenBlock(vector3df start, vector3df dirrection, float distance, vector3di& openBlock)
{
	/*
	Returns the coordinates of the last open block before a closed block.
	Ex. blocks placed by player should be placed in an open block spot
	dirrectly in front of a closed block.
	*/
	vector3df currentPos = start;
	dirrection.normalize();
	bool foundOpenBlock = false;
	float stepSize = 0.25f;

	for(float currentLength = 0.0f; currentLength < distance; currentLength += stepSize)
	{
		currentPos += dirrection * stepSize;

		if(IsBlockOpen(currentPos))
		{
			openBlock.set(vector3di((int)currentPos.X, (int)currentPos.Y, (int)currentPos.Z));
			foundOpenBlock = true;
		}
		else
		{
			break;
		}
	}

	return foundOpenBlock;
}

bool MapManager :: GetFirstClosedBlock(vector3df start, vector3df dirrection, float distance, vector3di& closedBlock)
{
	vector3df currentPos = start;
	dirrection.normalize();
	bool foundClosedBlock = false;
	float stepSize = 0.25f;

	for(float currentLength = 0.0f; currentLength < distance; currentLength += stepSize)
	{
		currentPos += dirrection * stepSize;
		if(!IsBlockOpen(currentPos))
		{
			closedBlock.set(vector3di((int)currentPos.X, (int)currentPos.Y, (int)currentPos.Z));
			foundClosedBlock = true;
			break;
		}
	}

	return foundClosedBlock;
}

bool MapManager :: GetPointOfMapCollision(vector3df start, vector3df dirrection, float distance, vector3df& collisionPoint)
{
	vector3df currentPos = start;
	dirrection.normalize();
	float stepSize = 0.05f;

	for(float currentLength = 0.0f; currentLength < distance; currentLength += stepSize)
	{
		currentPos += dirrection * stepSize;

		// found collision:
		if(IsBlockSolid(currentPos))
		{
			collisionPoint = currentPos;
			return true;
		}
	}

	return false;
}
