#ifndef AMMO_ITEM_H
#define AMMO_ITEM_H

#include "BaseItem.h"
#include "AmmoItemTemplate.h"
struct Damage;

class AmmoItem : public BaseItem
{
public:
	AmmoItem(int setID, AmmoItemTemplate* setTemplate);
	~AmmoItem(){}

	Damage* GetDamage() {return &((AmmoItemTemplate*)pTemplate)->damage;}

};
#endif
