#include "BlockManager.h"
#include "SimpleBlock.h"
#include "Damage.h"
#include "ItemManager.h"
#include <iostream>
#include <assert.h>


BlockManager* BlockManager :: Instance()
{
	static BlockManager instance;
	return &instance;
}

BlockManager :: BlockManager()
{
	InitBlockTemplates();
	InitSimpleBlocks();
}

BlockManager :: ~BlockManager()
{
	for(std::map<int, BaseBlock*>::iterator it = blockInstances.begin(); it != blockInstances.end(); it++)
		delete (*it).second;
}

void BlockManager :: InitBlockTemplates()
{
	// Sand Block
	blockTemplates[BLOCK_SAND].textureCoords.set(vector2df(0.05f + (1.0/640)*5.0f ));
	blockTemplates[BLOCK_SAND].damageType = DAMAGE_TYPE_SOIL;
	blockTemplates[BLOCK_SAND].dropID = ITEM_SAND_BLOCK;
	blockTemplates[BLOCK_SAND].dropAmount = 1;
	blockTemplates[BLOCK_SAND].buildItemIDs.push_back(ITEM_SAND_BLOCK);
	blockTemplates[BLOCK_SAND].buildItemAmounts.push_back(1);


	// Concrete Block:
	blockTemplates[BLOCK_CONCRETE].textureCoords.set(vector2df(0.1 + (1.0/640)*7.0f,0));
	blockTemplates[BLOCK_CONCRETE].damageType = DAMAGE_TYPE_STONE;
	blockTemplates[BLOCK_CONCRETE].dropID = ITEM_CONCRETE_BLOCK;
	blockTemplates[BLOCK_CONCRETE].dropAmount = 1;

	// Metal Block:
	blockTemplates[BLOCK_METAL].textureCoords.set(vector2df(0.15 + (1.0/640)*9.0f,0));
	blockTemplates[BLOCK_METAL].damageType = DAMAGE_TYPE_METAL;
	blockTemplates[BLOCK_METAL].dropID = ITEM_METAL_BLOCK;
	blockTemplates[BLOCK_METAL].dropAmount = 1;

	// Wood Block:
	blockTemplates[BLOCK_WOOD].textureCoords.set(vector2df(0.20 + (1.0/640)*11.0f ,0));
	blockTemplates[BLOCK_WOOD].damageType = DAMAGE_TYPE_WOOD;
	blockTemplates[BLOCK_WOOD].dropID = ITEM_WOOD_BLOCK;
	blockTemplates[BLOCK_WOOD].dropAmount = 1;

	// ScrapMetal Block:
	blockTemplates[BLOCK_SCRAP_METAL].textureCoords.set(vector2df(0.25 + (1.0/640)*13.0f,0));
	blockTemplates[BLOCK_SCRAP_METAL].damageType = DAMAGE_TYPE_METAL;
	blockTemplates[BLOCK_SCRAP_METAL].dropID = ITEM_SCRAP_METAL_BLOCK;
	blockTemplates[BLOCK_SCRAP_METAL].dropAmount = 1;

	// Asphalt Block:
	blockTemplates[BLOCK_ASPHALT].textureCoords.set(vector2df(0.30 + (1.0/640) * 15.0f,0));
	blockTemplates[BLOCK_ASPHALT].damageType = DAMAGE_TYPE_STONE;
	blockTemplates[BLOCK_ASPHALT].dropID = ITEM_ASPHALT_BLOCK;
	blockTemplates[BLOCK_ASPHALT].dropAmount = 1;

	// Brick Block:
	blockTemplates[BLOCK_BRICK].textureCoords.set(vector2df(0.35 + (1.0/640) * 17.0f,0));
	blockTemplates[BLOCK_BRICK].damageType = DAMAGE_TYPE_STONE;
	blockTemplates[BLOCK_BRICK].dropID = ITEM_BRICK_BLOCK;
	blockTemplates[BLOCK_BRICK].dropAmount = 1;

	// Green Concrete Block:
	blockTemplates[BLOCK_GREEN_CONCRETE].textureCoords.set(vector2df(0.40 + (1.0/640) * 19.0f,0));
	blockTemplates[BLOCK_GREEN_CONCRETE].damageType = DAMAGE_TYPE_STONE;
	blockTemplates[BLOCK_GREEN_CONCRETE].dropID = ITEM_GREEN_CONCRETE_BLOCK;
	blockTemplates[BLOCK_GREEN_CONCRETE].dropAmount = 1;

	// Blue Concrete Block:
	blockTemplates[BLOCK_BLUE_CONCRETE].textureCoords.set(vector2df(0.45 + (1.0/640) * 21.0f,0));
	blockTemplates[BLOCK_BLUE_CONCRETE].damageType = DAMAGE_TYPE_STONE;
	blockTemplates[BLOCK_BLUE_CONCRETE].dropID = ITEM_BLUE_CONCRETE_BLOCK;
	blockTemplates[BLOCK_BLUE_CONCRETE].dropAmount = 1;
}

void BlockManager :: InitSimpleBlocks()
{
	Register(new SimpleBlock(BLOCK_AIR,				&blockTemplates[BLOCK_AIR]));
	Register(new SimpleBlock(BLOCK_SAND,			&blockTemplates[BLOCK_SAND]));
	Register(new SimpleBlock(BLOCK_CONCRETE,		&blockTemplates[BLOCK_CONCRETE]));
	Register(new SimpleBlock(BLOCK_METAL,			&blockTemplates[BLOCK_METAL]));
	Register(new SimpleBlock(BLOCK_WOOD,			&blockTemplates[BLOCK_WOOD]));
	Register(new SimpleBlock(BLOCK_SCRAP_METAL,		&blockTemplates[BLOCK_SCRAP_METAL]));
	Register(new SimpleBlock(BLOCK_ASPHALT,			&blockTemplates[BLOCK_ASPHALT]));
	Register(new SimpleBlock(BLOCK_BRICK,			&blockTemplates[BLOCK_BRICK]));
	Register(new SimpleBlock(BLOCK_GREEN_CONCRETE,	&blockTemplates[BLOCK_GREEN_CONCRETE]));
	Register(new SimpleBlock(BLOCK_BLUE_CONCRETE,	&blockTemplates[BLOCK_BLUE_CONCRETE]));

}

// -----------------------------------------------------------------------
// Manage Block Instances:
// -----------------------------------------------------------------------
BaseBlock* BlockManager :: GetBlock(int id) const
{
	//find the entity
	std::map<int, BaseBlock*> :: const_iterator entity = blockInstances.find(id);

	if(entity == blockInstances.end())
	{
		std::cout << "Invalid Block ID: " << id << std::endl;
		assert(entity != blockInstances.end());
	}


	return entity->second;
}

void BlockManager :: Remove(BaseBlock* b)
{
	blockInstances.erase( blockInstances.find(b->ID()) );
}

void  BlockManager :: Register(BaseBlock* b)
{
	blockInstances.insert(std::make_pair(b->ID(), b));
}

// -----------------------------------------------------------------------
// Accessing Block Build Data:
// -----------------------------------------------------------------------
int BlockManager :: GetNumBuildComponents(int blockTemplateID)
{
	return blockTemplates[blockTemplateID].buildItemIDs.size();
}

int BlockManager :: GetBuildComponentID(int blockTemplateID, int index)
{
	return blockTemplates[blockTemplateID].buildItemIDs[index];
}

int BlockManager :: GetBuildComponentAmount(int blockTemplateID, int index)
{
	return blockTemplates[blockTemplateID].buildItemAmounts[index];
}
