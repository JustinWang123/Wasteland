#ifndef THROWN_WEAPON_ITEM_TEMPLATE_H
#define THROWN_WEAPON_ITEM_TEMPLATE_H

#include "BaseItemTemplate.h"
#include "Damage.h"

struct ThrownWeaponItemTemplate : public BaseItemTemplate
{
	Damage damage;
	float delay;
	float range;
};
#endif
