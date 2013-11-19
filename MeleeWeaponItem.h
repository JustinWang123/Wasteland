#ifndef MELEE_WEAPON_ITEM_H
#define MELEE_WEAPON_ITEM_H

#include "BaseItem.h"
#include "MeleeWeaponItemTemplate.h"

class MeleeWeaponItem : public BaseItem
{
public:
	MeleeWeaponItem(MeleeWeaponItemTemplate* setTemplate);
	~MeleeWeaponItem(){}

	virtual void Update();
	virtual void Use(CharacterInstance* owner);

	float	GetDPS() {return (float)GetDamage().type[DAMAGE_TYPE_FLESH] / (float)GetDelay();}

	Damage  GetDamage() {return GetTemplate()->damage;}
    int     GetDelay() {return GetTemplate()->delay;}
    int     GetRange() {return GetTemplate()->range;}

protected:
	MeleeWeaponItemTemplate* GetTemplate() {return (MeleeWeaponItemTemplate*)pTemplate;}

	float currentUseDelay;
};
#endif
