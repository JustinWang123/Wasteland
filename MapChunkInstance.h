#ifndef MAP_CHUNK_H
#define MAP_CHUNK_H

#include "IrrWrapper.h"
#include "BaseInstance.h"
#include <string>
#include <fstream>
class BaseBlock;

const int CHUNK_HEIGHT = 64;
const int CHUNK_SIZE = 16;
const int NULL_BLOCK = -1;

class MapChunkInstance : public BaseInstance
{
public:
	MapChunkInstance(vector3di pos);
	~MapChunkInstance();

	virtual bool HandleMessage(Telegram& msg);

	void SaveChunkAsOcTree();
	void LoadChunkAsOcTree();

	void InitSceneNode();
	void RebuildSceneNode();

	vector3di	GetPositionInt() {return position;}

	void		Activate();
	void		Deactivate();

	void		BuildBlock(vector3di index, int type);
	int			GetBlockID(vector3di index);
	bool		IsBlockSolid(vector3di index);
	virtual bool IsPosSolid(vector3df pos);

	bool		DamageBlock(vector3di index, int amount);
	int			GetBlockHP(vector3di index);

private:
	// Private methods for loading and saving the chunk:
	void GenerateDefaultChunk();
	void LoadOctreeString(std::ifstream &file, int fromX, int toX, int fromY, int toY, int fromZ, int toZ);
	std::string CreateOctreeString(int fromX, int toX, int fromY, int toY, int fromZ, int toZ);

	// Private methods for creating the chunk mesh:
	void GenerateMesh();
	void AddFace(int blockID, vector3df v1, vector3df v2, vector3df v3, vector3df v4, SColor color, int &currentIndex, int &currentVertex, SMeshBuffer* buffer);

	// Private methods for accessing the block data:
	void SetBlockID(vector3di index, int ID);
	void SetBlockHP(vector3di index, int value);
	bool IsInBounds(vector3df pos);
	bool IsInBounds(vector3di index);
	bool IsSimpleBlock(vector3di index);

	// Block Data:
	unsigned char blockID[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
	unsigned char blockHealth[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];

	// Chunk Data:
	vector3di position; // position of first block (in both world coords, and map data)
	IMeshSceneNode* sceneNode;
	SMesh* mesh;

	bool isActive;
};

#endif
