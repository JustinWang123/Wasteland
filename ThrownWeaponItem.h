#ifndef THROWN_WEAPON_ITEM_H
#define THROWN_WEAPON_ITEM_H

#include "BaseItem.h"
#include "ThrownWeaponItemTemplate.h"

class ThrownWeaponItem : public BaseItem
{
public:
	ThrownWeaponItem(ThrownWeaponItemTemplate* setTemplate);
	~ThrownWeaponItem(){}

	virtual void Update();
	virtual void Use(CharacterInstance* owner);

    int     GetRange() {return GetTemplate()->range;}

protected:
	ThrownWeaponItemTemplate* GetTemplate() {return (ThrownWeaponItemTemplate*)pTemplate;}
};
#endif

