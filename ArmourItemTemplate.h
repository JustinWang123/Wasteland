#ifndef ARMOUR_ITEM_TEMPLATE_H
#define ARMOUR_ITEM_TEMPLATE_H

#include "BaseItemTemplate.h"

enum armourSlots
{
	ARMOR_SLOT_HEAD,
	ARMOR_SLOT_CHEST,
	ARMOR_SLOT_LEGS,
	ARMOR_SLOT_FEET
};

struct ArmourItemTemplate : public BaseItemTemplate
{
	int slot;
	int maxDurability;
	int damageMitigationMod;
	int fireMitigationMod;
	int strengthMod;
	int speedMod;
	int perceptionMod;
	int staminaMod;
};
#endif
