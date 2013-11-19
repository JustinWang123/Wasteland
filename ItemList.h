#ifndef ITEM_LIST_H
#define ITEM_LIST_H

#include "ItemSlot.h"
#include <vector>

class ItemList
{
public:
	ItemList();
	~ItemList();

	void 		AddToList(BaseItem* item, int amount);

	void 		RemoveItemTypeFromInventory(int itemTemplateID, int amount);
	BaseItem* 	RemoveFromSlot(int slotIndex, int amount);
	void 		ClearSlot(int index);
	void 		ClearAll();

	int 		GetSize()  const;
	ItemSlot* 	GetSlot(int index)  const;
	BaseItem* 	GetSlotItem(int index) const;
	int 		GetSlotAmount(int index) const;
	float		GetTotalWeight() const;
	int 		AmountOfItemInInventory(int itemTemplateID) const;

private:
	void EraseSlot(int index);
	void DebugDisplayContents();

	std::vector<ItemSlot*> itemList;
};

#endif
