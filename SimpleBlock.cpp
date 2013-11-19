#include "SimpleBlock.h"
#include "MapManager.h"
#include "BaseBlockTemplate.h"

SimpleBlock :: SimpleBlock(int setID, BaseBlockTemplate* BaseBlockTemplate):
	BaseBlock(setID)
{
	pTemplate = BaseBlockTemplate;
}

// -----------------------------------------------------------------------------------
// Electrical Accessors:
// -----------------------------------------------------------------------------------
bool SimpleBlock :: IsElectrical()
{
	return pTemplate->isElectricalComponenet;
}

void SimpleBlock :: ConsumePower(float amount)
{
}

float SimpleBlock :: ProvidePower(float desiredAmount)
{
	return 0.0f;
}


// -----------------------------------------------------------------------------------
// Get Accessors:
// -----------------------------------------------------------------------------------
vector2df SimpleBlock :: GetTextureCoords()
{
	return GetTemplate()->textureCoords;
}