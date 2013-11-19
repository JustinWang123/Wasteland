#ifndef MELEE_WEAPON_ITEM_TEMPLATE_H
#define MELEE_WEAPON_ITEM_TEMPLATE_H

#include "BaseItemTemplate.h"
#include "Damage.h"

struct MeleeWeaponItemTemplate : public BaseItemTemplate
{
	Damage damage;
	float delay;
	float range;
};
#endif
