#ifndef CONSUMABLE_ITEM_TEMPLATE_H
#define CONSUMABLE_ITEM_TEMPLATE_H

#include "BaseItemTemplate.h"

struct ConsumableItemTemplate : public BaseItemTemplate
{
	int health;
	int hunger;
	int thirst;
};
#endif