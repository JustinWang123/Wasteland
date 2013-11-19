#include "MapChunkInstance.h"
#include "InstanceManager.h"
#include "GameManager.h"
#include "ItemInstance.h"
#include "ZombieInstance.h"
#include "BlockManager.h"
#include "SimpleBlock.h"
#include "ProjectileInstance.h"
#include "MessageManager.h"
#include "ItemManager.h"
#include <fstream>
#include <map>
#include <iostream>


using namespace std;

MapChunkInstance :: MapChunkInstance(vector3di pos)
{
	sceneNode = 0;
	position = vector3di((int)pos.X, (int)pos.Y, (int)pos.Z);
	bitMask = BITMASK_WORLD;

	for(int x = 0; x < CHUNK_SIZE; x++)
		for(int y = 0; y < CHUNK_HEIGHT; y++)
			for(int z = 0; z < CHUNK_SIZE; z++)
				blockID[x][y][z] = 1; // blocks are 1 by default so they are solid

	isActive = false;
}

MapChunkInstance :: ~MapChunkInstance()
{
	if(sceneNode != 0)
		sceneNode->remove();

	// Only save if creative mode is on:
	if(GManager->IsCreativeModeOn())
	{
		//SaveChunk();
		SaveChunkAsOcTree();
	}
}

bool MapChunkInstance :: HandleMessage(Telegram& msg)
{
	if(msg.msg == MESSAGE_TYPE_DAMAGE_HIT)
	{
		DamageHitMessage* message = (DamageHitMessage*)msg.extraInfo;
		message->position += message->heading * 0.01f; // This gurentees that the collision occurs inside of the block that was hit

		vector3di index(message->position.X - position.X, message->position.Y - position.Y, message->position.Z - position.Z);

		vector3df particleHeading = -message->heading;
		particleHeading.rotateXYBy(0.1, message->position);
		InstanceMgr->CreateProjectileInstance(2, message->position, particleHeading, 0);

		particleHeading = -message->heading;
		particleHeading.rotateXYBy(-0.1, message->position);
		InstanceMgr->CreateProjectileInstance(2, message->position, particleHeading, 0);

		particleHeading = -message->heading;
		particleHeading.rotateXZBy(0.1, message->position);
		InstanceMgr->CreateProjectileInstance(2, message->position, particleHeading, 0);

		particleHeading = -message->heading;
		particleHeading.rotateXZBy(-0.1, message->position);
		InstanceMgr->CreateProjectileInstance(2, message->position, particleHeading, 0);

		particleHeading = -message->heading;
		particleHeading.rotateYZBy(0.1, message->position);
		InstanceMgr->CreateProjectileInstance(2, message->position, particleHeading, 0);

		particleHeading = -message->heading;
		particleHeading.rotateYZBy(-0.1, message->position);
		InstanceMgr->CreateProjectileInstance(2, message->position, particleHeading, 0);

		// Damage Block:

		int blockID = GetBlockID(index);
		BaseBlock* block = BlockMgr->GetBlock(blockID);
		DamageBlock(index, message->damage.type[block->GetDamageType()]);

		return true;
	}
	return false;
}

void MapChunkInstance :: GenerateDefaultChunk()
{
	cout << "Generating Default Chunk: " << position.X << ", " << position.Z << endl;

	for(int x = 0; x < CHUNK_SIZE; x++)
		for(int y = 0; y < CHUNK_HEIGHT; y++)
			for(int z = 0; z < CHUNK_SIZE; z++)
				if(y < 20)
				{
					blockID[x][y][z] = 1;
					blockHealth[x][y][z] = 100;
				}
				else
				{
					blockID[x][y][z] = 0;
					blockHealth[x][y][z] = 0;
				}
}

void MapChunkInstance :: LoadChunkAsOcTree()
{
	cout << "Loading Chunk as Octree: " << position.X << ", " << position.Z << endl;

	ifstream file; // Create output file:
    std::string name = "OctreeChunks/" + IRR->ToString(position.X) + " " + IRR->ToString(position.Z);

	file.open ( name.c_str() ); // create or overwrite file:

	if(file)
	{
		std::string str;

		// Load octrees
		for(int firstY = 0; firstY < 64; firstY += 16)
		{
			file >> str;
			if(str == "OCTREE")
			{
				LoadOctreeString(file, 0, 16, firstY, firstY + 16, 0, 16);
			}
			else
			{
				for(int x = 0; x < 16; x++)
					for(int y = firstY; y < firstY + 16; y++)
						for(int z = 0; z < 16; z++)
						{
							blockID[x][y][z] = atoi(str.c_str());
							blockHealth[x][y][z] = 100; // Change eventually
						}
			}
		}

		if( !file.eof() )
		{
			int templateID;
			int anountOfItem;
			int count = 0;
			vector3df tempPosVector;
			vector3df tempRotVector;
			file >> count;
			if(count != 0)
			{
				for(int i = 0; i < count; i++)
				{
					file >> templateID;
					file >> anountOfItem;
					file >> tempPosVector.X;
					file >> tempPosVector.Y;
					file >> tempPosVector.Z;
					file >> tempRotVector.X;
					file >> tempRotVector.Y;
					file >> tempRotVector.Z;

					InstanceMgr->CreateItemInstance(templateID, anountOfItem, tempPosVector, tempRotVector);
				}
			}
		}

		if( !file.eof() )
		{
			int count = 0;
			vector3df tempPosVector;
			file >> count;

			if(count != 0)
			{
				for(int i = 0; i < count; i++)
				{
					file >> tempPosVector.X;
					file >> tempPosVector.Y;
					file >> tempPosVector.Z;

					//InstanceMgr->CreateNonPlayerInstance(CHARACTER_INSTANCE_TEMPLATE_ZOMBIE, FACTION_ID_ZOMBIE, tempPosVector);
				}
			}
		}

	}
	else
	{
		GenerateDefaultChunk();
	}

	file.close();

}

void MapChunkInstance :: SaveChunkAsOcTree()
{
	cout << "Saving Chunk as Octree: " << position.X << ", " << position.Z << endl;

	ofstream myfile; // Creat output file:

	std::string name = "OctreeChunks/" + IRR->ToString(position.X) + " " + IRR->ToString(position.Z);
	myfile.open (name.c_str()); // create or overwrite file:

	myfile << CreateOctreeString(0, 16, 0, 16, 0, 16);
	myfile << CreateOctreeString(0, 16, 16, 32, 0, 16);
	myfile << CreateOctreeString(0, 16, 32, 48, 0, 16);
	myfile << CreateOctreeString(0, 16, 48, 64, 0, 16);

	int count = 0;

	// Count item instances:
	for(std::vector<ItemInstance*>::iterator it = InstanceMgr->GetItemInstances()->begin(); it != InstanceMgr->GetItemInstances()->end(); it++)
		if((*it)->GetPosition().X >= position.X
		&& (*it)->GetPosition().Z >= position.Z
		&& (*it)->GetPosition().X < position.X + CHUNK_SIZE
		&& (*it)->GetPosition().Z < position.Z + CHUNK_SIZE)
		{
			count++;
		}

	myfile << count << " ";

	// Save all item instances:
	for(std::vector<ItemInstance*>::iterator it = InstanceMgr->GetItemInstances()->begin(); it != InstanceMgr->GetItemInstances()->end(); it++)
		if((*it)->GetPosition().X >= position.X
		&& (*it)->GetPosition().Z >= position.Z
		&& (*it)->GetPosition().X < position.X + CHUNK_SIZE
		&& (*it)->GetPosition().Z < position.Z + CHUNK_SIZE)
		{
			myfile << (*it)->GetTemplateID() << " ";
			myfile << (*it)->GetItemAmount() << " ";
			myfile << (*it)->GetPosition().X << " ";
			myfile << (*it)->GetPosition().Y << " ";
			myfile << (*it)->GetPosition().Z << " ";
			myfile << (*it)->GetRotation().X << " ";
			myfile << (*it)->GetRotation().Y << " ";
			myfile << (*it)->GetRotation().Z << " ";
		}

	// Count all npc instances:
	std::list<int> charactersInChunk;

	InstanceMgr->GetCharactersInBox(charactersInChunk,
									vector3df(position.X, position.Y, position.Y),
									vector3df(position.X, position.Y, position.Y) + vector3df(CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE));

	myfile << charactersInChunk.size() << " ";

	// Save all npc instances:
	for(std::list<int>::iterator it = charactersInChunk.begin(); it != charactersInChunk.end(); it++)
	{
		myfile << InstanceMgr->GetCharacterFromID( *it )->GetFeetPosition().X << " ";
		myfile << InstanceMgr->GetCharacterFromID( *it )->GetFeetPosition().Y << " ";
		myfile << InstanceMgr->GetCharacterFromID( *it )->GetFeetPosition().Z << " ";
	}


	myfile.close();
}

void MapChunkInstance :: LoadOctreeString(std::ifstream &file, int fromX, int toX, int fromY, int toY, int fromZ, int toZ)
{
	/*
	  7 -- 8
	 5 -- 6|
	 | |  ||
	 | 3--|4
	 1/---2

	*/


	std::string str;

	// Grab 1:
	file >> str;
	if(str == "OCTREE")
	{
		LoadOctreeString(file, fromX, fromX + (toX - fromX)/2, fromY, fromY + (toY - fromY)/2, fromZ, fromZ + (toZ - fromZ)/2);
	}
	else
	{
		for(int x = fromX; x < fromX + (toX - fromX)/2; x++)
			for(int y = fromY; y < fromY + (toY - fromY)/2; y++)
				for(int z = fromZ; z < fromZ + (toZ - fromZ)/2; z++)
				{
					blockID[x][y][z] = atoi(str.c_str());
					blockHealth[x][y][z] = 100;
				}
	}

	// Grab 2:
	file >> str;
	if(str == "OCTREE")
	{
		LoadOctreeString(file, fromX + (toX - fromX)/2, toX, fromY, fromY + (toY - fromY)/2, fromZ, fromZ + (toZ - fromZ)/2);
	}
	else
	{
		for(int x = fromX + (toX - fromX)/2; x < toX; x++)
			for(int y = fromY; y < fromY + (toY - fromY)/2; y++)
				for(int z = fromZ; z < fromZ + (toZ - fromZ)/2; z++)
				{
					blockID[x][y][z] = atoi(str.c_str());
					blockHealth[x][y][z] = 100;
				}
	}

	// Grab 3:
	file >> str;
	if(str == "OCTREE")
	{
		LoadOctreeString(file, fromX, fromX + (toX - fromX)/2, fromY, fromY + (toY - fromY)/2, fromZ + (toZ - fromZ)/2, toZ);
	}
	else
	{
		for(int x = fromX; x < fromX + (toX - fromX)/2; x++)
			for(int y = fromY; y < fromY + (toY - fromY)/2; y++)
				for(int z = fromZ + (toZ - fromZ)/2; z < toZ; z++)
				{
					blockID[x][y][z] = atoi(str.c_str());
					blockHealth[x][y][z] = 100;
				}
	}

	// Grab 4:
	file >> str;
	if(str == "OCTREE")
	{
		LoadOctreeString(file, fromX + (toX - fromX)/2, toX, fromY, fromY + (toY - fromY)/2, fromZ + (toZ - fromZ)/2, toZ);
	}
	else
	{
		for(int x = fromX + (toX - fromX)/2; x < toX; x++)
			for(int y = fromY; y < fromY + (toY - fromY)/2; y++)
				for(int z = fromZ + (toZ - fromZ)/2; z < toZ; z++)
				{
					blockID[x][y][z] = atoi(str.c_str());
					blockHealth[x][y][z] = 100;
				}
	}

	// Grab 5:
	file >> str;
	if(str == "OCTREE")
	{
		LoadOctreeString(file, fromX, fromX + (toX - fromX)/2, fromY + (toY - fromY)/2, toY, fromZ, fromZ + (toZ - fromZ)/2);
	}
	else
	{
		for(int x = fromX; x < fromX + (toX - fromX)/2; x++)
			for(int y = fromY + (toY - fromY)/2; y < toY; y++)
				for(int z = fromZ; z < fromZ + (toZ - fromZ)/2; z++)
				{
					blockID[x][y][z] = atoi(str.c_str());
					blockHealth[x][y][z] = 100;
				}
	}

	// Grab 6:
	file >> str;
	if(str == "OCTREE")
	{
		LoadOctreeString(file, fromX + (toX - fromX)/2, toX, fromY + (toY - fromY)/2, toY, fromZ, fromZ + (toZ - fromZ)/2);
	}
	else
	{
		for(int x = fromX + (toX - fromX)/2; x < toX; x++)
			for(int y = fromY + (toY - fromY)/2; y < toY; y++)
				for(int z = fromZ; z < fromZ + (toZ - fromZ)/2; z++)
				{
					blockID[x][y][z] = atoi(str.c_str());
					blockHealth[x][y][z] = 100;
				}
	}

	// Grab 7:
	file >> str;
	if(str == "OCTREE")
	{
		LoadOctreeString(file, fromX, fromX + (toX - fromX)/2, fromY + (toY - fromY)/2, toY, fromZ + (toZ - fromZ)/2, toZ);
	}
	else
	{
		for(int x = fromX; x < fromX + (toX - fromX)/2; x++)
			for(int y = fromY + (toY - fromY)/2; y < toY; y++)
				for(int z = fromZ + (toZ - fromZ)/2; z < toZ; z++)
				{
					blockID[x][y][z] = atoi(str.c_str());
					blockHealth[x][y][z] = 100;
				}
	}

	// Grab 8:
	file >> str;
	if(str == "OCTREE")
	{
		LoadOctreeString(file, fromX + (toX - fromX)/2, toX, fromY + (toY - fromY)/2, toY, fromZ + (toZ - fromZ)/2, toZ);
	}
	else
	{
		for(int x = fromX + (toX - fromX)/2; x < toX; x++)
			for(int y = fromY + (toY - fromY)/2; y < toY; y++)
				for(int z = fromZ + (toZ - fromZ)/2; z < toZ; z++)
				{
					blockID[x][y][z] = atoi(str.c_str());
					blockHealth[x][y][z] = 100;
				}
	}

}

std::string MapChunkInstance :: CreateOctreeString(int fromX, int toX, int fromY, int toY, int fromZ, int toZ)
{

	// Check if chunk is only a single block:
	if(fromX == toX && fromY == toY && fromZ == toZ)
	{
		// Assure we dont return values past end of blockID array:
		if(toX != CHUNK_SIZE && toY != CHUNK_SIZE && toZ != CHUNK_SIZE)
		{
			return IRR->ToString(blockID[fromX][fromY][fromZ]) + " ";
		}
		else
		{
			return "";
		}
	}

	// Check if the chunk is homogenious:
	int sameblockID = blockID[fromX][fromY][fromZ];
	bool isHomogenious = true;

	for(int x = fromX; x < toX; x++)
	{
		for(int y = fromY; y < toY; y++)
		{
			for(int z = fromZ; z < toZ; z++)
			{
				if(blockID[x][y][z] != sameblockID)
				{
					isHomogenious = false;
				}
			}
		}
	}

	if(isHomogenious)
	{
		return IRR->ToString(sameblockID) + " ";
	}


	// Chunk is not a single block and is not homogenious so result is a quadtree
	std::string result("");

	result += "OCTREE "; // Tag to indicate quadtree follows

	result += CreateOctreeString(fromX, fromX + (toX - fromX)/2, fromY, fromY + (toY - fromY)/2, fromZ, fromZ + (toZ - fromZ)/2);
	result += CreateOctreeString(fromX + (toX - fromX)/2, toX, fromY, fromY + (toY - fromY)/2, fromZ, fromZ + (toZ - fromZ)/2);

	result += CreateOctreeString(fromX, fromX + (toX - fromX)/2, fromY, fromY + (toY - fromY)/2, fromZ + (toZ - fromZ)/2, toZ);
	result += CreateOctreeString(fromX + (toX - fromX)/2, toX, fromY, fromY + (toY - fromY)/2, fromZ + (toZ - fromZ)/2, toZ);

	result += CreateOctreeString(fromX, fromX + (toX - fromX)/2, fromY + (toY - fromY)/2, toY, fromZ, fromZ + (toZ - fromZ)/2);
	result += CreateOctreeString(fromX + (toX - fromX)/2, toX, fromY + (toY - fromY)/2, toY, fromZ, fromZ + (toZ - fromZ)/2);

	result += CreateOctreeString(fromX, fromX + (toX - fromX)/2, fromY + (toY - fromY)/2, toY, fromZ + (toZ - fromZ)/2, toZ);
	result += CreateOctreeString(fromX + (toX - fromX)/2, toX, fromY + (toY - fromY)/2, toY, fromZ + (toZ - fromZ)/2, toZ);

	return result;
}

void MapChunkInstance :: InitSceneNode()
{
	mesh = new SMesh();

	GenerateMesh();

	sceneNode = IRR->sceneMgr->addMeshSceneNode(mesh, 0, BITMASK_WORLD);

	sceneNode->setPosition(vector3df(position.X, position.Y, position.Z));
	sceneNode->setMaterialTexture( 0, IRR->driver->getTexture("tex/BlockTextures.png"));
	sceneNode->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
	sceneNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	sceneNode->setMaterialFlag(EMF_LIGHTING, false);
	sceneNode->setMaterialFlag(video::EMF_BILINEAR_FILTER , false);
	sceneNode->setMaterialFlag(video::EMF_TRILINEAR_FILTER , false);
	sceneNode->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER , true);
	sceneNode->setMaterialFlag(EMF_FOG_ENABLE, false);
	sceneNode->setAutomaticCulling(EAC_OFF);

	isActive = true;
}

void MapChunkInstance :: RebuildSceneNode()
{
	GenerateMesh();
}

void MapChunkInstance :: AddFace(int blockID, vector3df pos1, vector3df pos2, vector3df pos3, vector3df pos4, SColor color, int &currentIndex, int &currentVertex, SMeshBuffer* buffer)
{
	int v1ID = currentVertex++;
	S3DVertex& v1 = buffer->Vertices[v1ID];
	v1.Pos.set(pos1);
	v1.Normal.set(vector3df (0,0,1));
	v1.Color = color;
	v1.TCoords.set(vector2df(((SimpleBlock*)BlockMgr->GetBlock(blockID))->GetTextureCoords().X, 0.0f));

	int v2ID = currentVertex++;
	S3DVertex& v2 = buffer->Vertices[v2ID];
	v2.Pos.set(pos2);
	v2.Normal.set(vector3df (0,0,1));
	v2.Color = color;
	v2.TCoords.set(vector2df(((SimpleBlock*)BlockMgr->GetBlock(blockID))->GetTextureCoords().X + 0.045f, 0.0f));

	int v3ID = currentVertex++;
	S3DVertex& v3 = buffer->Vertices[v3ID];
	v3.Pos.set(pos3);
	v3.Normal.set(vector3df (0,0,1));
	v3.Color = color;
	v3.TCoords.set(vector2df(((SimpleBlock*)BlockMgr->GetBlock(blockID))->GetTextureCoords().X + 0.045f, 1.0f));

	int v4ID = currentVertex++;
	S3DVertex& v4 = buffer->Vertices[v4ID];
	v4.Pos.set(pos4);
	v4.Normal.set(vector3df (0,0,1));
	v4.Color = color;
	v4.TCoords.set(vector2df(((SimpleBlock*)BlockMgr->GetBlock(blockID))->GetTextureCoords().X, 1.0f));

	buffer->Indices[currentIndex++] = v1ID;
	buffer->Indices[currentIndex++] = v4ID;
	buffer->Indices[currentIndex++] = v3ID;

	buffer->Indices[currentIndex++] = v1ID;
	buffer->Indices[currentIndex++] = v3ID;
	buffer->Indices[currentIndex++] = v2ID;
}

void MapChunkInstance :: GenerateMesh()
{

	SMeshBuffer* buffer;

	if(mesh->getMeshBufferCount() == 0)
	{
		buffer = new SMeshBuffer();
		mesh->addMeshBuffer(buffer);
		buffer->drop();
	}
	else
	{
		buffer = (SMeshBuffer*)sceneNode->getMesh()->getMeshBuffer(0);
		buffer->Vertices.set_used(0);
		buffer->Indices.set_used(0);
	}

	int currentVertex = 0;
	int currentIndex = 0;
	int numFaces = 0;

	for(int y = 0; y < CHUNK_HEIGHT; y++)
		for(int z = 0; z < CHUNK_SIZE; z++)
			for(int x = 0; x < CHUNK_SIZE; x++)
	 		{
				if(IsSimpleBlock(vector3di(x,y,z)))
				{
					// Bottem Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x,y-1,z)))
					{
						currentVertex += 4;
						currentIndex += 6;
					}

					// Top Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x,y+1,z)))
					{
						currentVertex += 4;
						currentIndex += 6; }

					// Front Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x,y,z-1)))
					{
						currentVertex += 4;
						currentIndex += 6;
					}

					// Back Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x,y,z+1)))
					{
						currentVertex += 4;
						currentIndex += 6;
					}

					// Left Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x-1,y,z)))
					{
						currentVertex += 4;
						currentIndex += 6;
					}

					// Right Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x+1,y,z)))
					{
						currentVertex += 4;
						currentIndex += 6;
					}
				}
			}

	buffer->Vertices.set_used(currentVertex);
	buffer->Indices.set_used(currentIndex);


	currentVertex = 0;
	currentIndex = 0;

	for(int y = 0; y < CHUNK_HEIGHT; y ++)
		for(int z = 0; z < CHUNK_SIZE; z++)
			for(int x = 0; x < CHUNK_SIZE; x++)
	 		{
				if(IsSimpleBlock(vector3di(x,y,z)))
				{
					// Bottem Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x,y-1,z)))
					{
						AddFace(GetBlockID(vector3di(x,y,z)) ,vector3df(x,y,z), vector3df(x,y,z+1), vector3df(x+1,y,z+1), vector3df(x+1,y,z), SColor(255,128,128,128), currentIndex, currentVertex, buffer);
						numFaces++;
					}

					// Top Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x,y+1,z)))
					{
						AddFace(GetBlockID(vector3di(x,y,z)) ,vector3df(x,y+1,z), vector3df(x+1,y+1,z), vector3df(x+1,y+1,z+1), vector3df(x,y+1,z+1), SColor(255,255,255,255),currentIndex, currentVertex, buffer);
						numFaces++;
					}

					// Front Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x,y,z-1)))
					{
						AddFace(GetBlockID(vector3di(x,y,z)) ,vector3df(x,y,z), vector3df(x+1,y,z), vector3df(x+1,y+1,z), vector3df(x,y+1,z), SColor(255,192,192,192),currentIndex, currentVertex, buffer);
						numFaces++;
					}

					// Back Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x,y,z+1)))
					{
						AddFace(GetBlockID(vector3di(x,y,z)) ,vector3df(x,y,z+1), vector3df(x,y+1,z+1), vector3df(x+1,y+1,z+1), vector3df(x+1,y,z+1), SColor(255,192,192,192),currentIndex, currentVertex, buffer);
						numFaces++;
					}

					// Left Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x-1,y,z)))
					{
						AddFace(GetBlockID(vector3di(x,y,z)) ,vector3df(x,y,z), vector3df(x,y+1,z), vector3df(x,y+1,z+1), vector3df(x,y,z+1),  SColor(255,192,192,192),currentIndex, currentVertex, buffer);
						numFaces++;
					}

					// Right Face:
					if(!MapMgr->IsBlockSolid(position + vector3di(x+1,y,z)))
					{
						AddFace(GetBlockID(vector3di(x,y,z)) ,vector3df(x+1,y,z), vector3df(x+1,y,z+1), vector3df(x+1,y+1,z+1), vector3df(x+1,y+1,z),  SColor(255,192,192,192),currentIndex, currentVertex, buffer);
						numFaces++;
					}
				}
			}

}

bool MapChunkInstance :: IsBlockSolid(vector3di index)
{
	return GetBlockID(index) != 0;
}

bool MapChunkInstance :: IsPosSolid(vector3df pos)
{
	if( IsInBounds(pos) )
	{
		vector3di testPos( (int)pos.X - position.X, (int)pos.Y - position.Y, (int)pos.Z - position.Z);
		return IsBlockSolid(testPos);
	}
	else
	{
		return false;
	}
}

int	MapChunkInstance :: GetBlockID(vector3di index)
{
	if(IsInBounds(index))
	{
		return (int)blockID[index.X][index.Y][index.Z];
	}
	else
	{
		return NULL_BLOCK;
	}
}

int	MapChunkInstance :: GetBlockHP(vector3di pos)
{
	if(IsInBounds(pos))
	{
		return (int)blockHealth[pos.X][pos.Y][pos.Z];
	}
	else
	{
		return 0;
	}

}

void MapChunkInstance :: SetBlockID(vector3di index, int ID)
{
	if(IsInBounds(index))
	{
		blockID[index.X][index.Y][index.Z] = ID;
		RebuildSceneNode();
	}
}

void MapChunkInstance :: SetBlockHP(vector3di index, int value)
{
	if(IsInBounds(index))
	{
		blockHealth[index.X][index.Y][index.Z] = value;
	}
}

bool MapChunkInstance :: DamageBlock(vector3di index, int amount)
{
	if(IsInBounds(index))
	{
		// If the block is destroyed return true:
		if(GetBlockHP(index) <= amount)
		{
			SetBlockID(index,0);
			return true;
		}
		else
		{
			SetBlockHP(index, GetBlockHP(index) - amount);
		}
	}

	return false;
}

void MapChunkInstance :: BuildBlock(vector3di index, int type)
{
	SetBlockID(index, type);
	SetBlockHP(index, 100);
}

bool MapChunkInstance :: IsInBounds(vector3df pos)
{
	return pos.X > position.X && pos.X < position.X + CHUNK_SIZE
		&& pos.Y > position.Y && pos.Y < position.Y + CHUNK_HEIGHT
		&& pos.Z > position.Z && pos.Z < position.Z + CHUNK_SIZE;
}

bool MapChunkInstance :: IsInBounds(vector3di index)
{
	return index.X >= 0 && index.X < CHUNK_SIZE
		&& index.Y >= 0 && index.Y < CHUNK_HEIGHT
		&& index.Z >= 0 && index.Z < CHUNK_SIZE;
}

void MapChunkInstance :: Activate()
{
	if(sceneNode)
		sceneNode->setVisible(true);

	isActive = true;
}

void MapChunkInstance :: Deactivate()
{
	if(sceneNode)
		sceneNode->setVisible(false);

	isActive = false;
}

bool MapChunkInstance :: IsSimpleBlock(vector3di index)
{
	if(IsBlockSolid(index) && GetBlockID(index) != NULL_BLOCK)
	{
		int id = GetBlockID(index);
		return BlockMgr->GetBlock(id )->IsSimpleBlock();
	}
	else
	{
		return false;
	}

}
