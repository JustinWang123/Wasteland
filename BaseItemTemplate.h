#ifndef BASE_ITEM_TEMPLATE
#define BASE_ITEM_TEMPLATE

#include "Perameters.h"
#include "IrrWrapper.h"
#include <string>

struct BaseItemTemplate
{
	int id;
	std::string name;
	ItemType type;
	ITexture* invTex;
	IMesh* mesh;
	int fuelValue;
	vector3df equipPos;
	vector3df equipRot;
	float equipScale;
	bool isStackable;
	bool isUnique;
	float weight;
};

#endif
