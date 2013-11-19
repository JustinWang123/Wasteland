#ifndef BASE_BLOCK_TEMPLATE_H
#define BASE_BLOCK_TEMPLATE_H

#include "IrrWrapper.h"
#include <vector>

struct BaseBlockTemplate
{
	int			damageType;
	int			dropID;
	int			dropAmount;
	bool		isElectricalComponenet;
	vector2df	textureCoords;

	std::vector<int> buildItemIDs;
	std::vector<int> buildItemAmounts;
};

#endif