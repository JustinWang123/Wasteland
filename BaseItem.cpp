#include "BaseItem.h"

int BaseItem :: nextID = 1000;

BaseItem :: BaseItem(BaseItemTemplate* setTemplate)
{
	pTemplate = setTemplate;
	itemID = nextID;
	nextID++;
}

BaseItem :: BaseItem(int setID, BaseItemTemplate* setTemplate)
{
	pTemplate = setTemplate;
	itemID = setID;
}


