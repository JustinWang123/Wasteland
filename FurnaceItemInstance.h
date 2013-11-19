#pragma
#ifndef FURNACE_ITEM_INSTANCE_H
#define FURNACE_ITEM_INSTANCE_H

#include "ItemInstance.h"
#include "ItemSlot.h"

class FurnaceItemInstance : public  ItemInstance
{
public:
	FurnaceItemInstance(int itemID, vector3df position, vector3df rotation);
	~FurnaceItemInstance();

	virtual void Update();
	void DrawInterface();

	int			fuel;
	int			fuelTowardsProduct;
	ItemSlot	fuelSlot;
	ItemSlot	rawSlot;
	ItemSlot	productSlot;
};
#endif