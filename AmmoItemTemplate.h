#ifndef AMMO_ITEM_TEMPLATE_H
#define AMMO_ITEM_TEMPLATE_H

#include "BaseItemTemplate.h"
#include "Damage.h"

struct AmmoItemTemplate : public BaseItemTemplate
{
	Damage damage;
};
#endif
