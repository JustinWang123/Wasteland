#ifndef BLOCK_MANAGER_H
#define BLOCK_MANAGER_H

#include "BaseBlockTemplate.h"
#include "ComplexBlockTemplate.h"
#include "BaseBlock.h"
#include <map>

const int MAX_BLOCK_TEMPLATES = 100;

class BlockManager
{
public:
	static BlockManager*		Instance();
								~BlockManager();

	// Manage Block Instances:
	void						Register(BaseBlock* b);
	BaseBlock*					GetBlock(int id) const;
	void						Remove(BaseBlock* e);

	// Accessing Block Build Data:
	int							GetNumBuildComponents(int blockTemplateID);
	int							GetBuildComponentID(int blockTemplateID, int index);
	int							GetBuildComponentAmount(int blockTemplateID, int index);

private:
								BlockManager();
	void						InitBlockTemplates();
	void						InitSimpleBlocks();

	
	std::map<int, BaseBlock*>	blockInstances;
	BaseBlockTemplate			blockTemplates[MAX_BLOCK_TEMPLATES];
};

#define BlockMgr BlockManager::Instance()
#endif