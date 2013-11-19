#ifndef PROJECTILE_WEAPON_ITEM_TEMPLATE_H
#define PROJECTILE_WEAPON_ITEM_TEMPLATE_H

#include "BaseItemTemplate.h"

enum FireMode
{
	FIRE_MODE_SEMI_AUTO,
	FIRE_MODE_FULL_AUTO,
	FIRE_MODE_BURST
};

enum CartridgeType
{
	CARTRIDGE_TYPE_SLUG,
	CARTRIDGE_TYPE_SHOT
};

struct ProjectileWeaponItemTemplate : public BaseItemTemplate
{
	int clipSize;
	float reloadTime;
	float shootDelay;
	FireMode fireMode;
	CartridgeType cartridgeType;
	int ammoItemTemplateID;
	int maxDurability;
	int range;
};
#endif
