#include "BaseBlock.h"

int BaseBlock :: nextID = 1000;

BaseBlock :: BaseBlock()
{
	id = nextID;
	nextID++;
}

BaseBlock :: BaseBlock(int setID)
{
	id = setID;
}

bool BaseBlock :: IsSimpleBlock()
{
	return id < LAST_SIMPLE_BLOCK_ID;
}

// Get Accessors:
int BaseBlock :: GetDropID()
{
	return pTemplate->dropID;
}

int	BaseBlock :: GetDropAmount()
{
	return pTemplate->dropAmount;
}

int BaseBlock :: GetDamageType()
{
	return pTemplate->damageType;
}

