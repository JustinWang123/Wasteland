#ifndef TOOL_ITEM_TEMPLATE_H
#define TOOL_ITEM_TEMPLATE_H

#include "BaseItemTemplate.h"
#include "Damage.h"

struct ToolItemTemplate : public BaseItemTemplate
{
	Damage damage;
	float delay;
	float range;
	int maxDurability;
};
#endif