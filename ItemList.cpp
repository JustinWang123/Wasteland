#include "ItemList.h"
#include <iostream>

ItemList :: ItemList()
{
}

ItemList :: ~ItemList()
{
	ClearAll();
}

void ItemList :: AddToList(BaseItem* item, int amount)
{
	// If the item is stackable, search for an existing ItemSlot containing that item
	if(item->GetIsStackable())
	{
		bool itemAdded = false;

		for(std::vector<ItemSlot*>::iterator it = itemList.begin(); it != itemList.end(); it++)
			if( (*it)->GetTemplateID() == item->GetTemplateID() )
			{
				(*it)->AddToSlot(item, amount);
				itemAdded = true;
				break;
			}

		// If a stack was not found that create a new inventory slot to hold the stack:
		if(itemAdded == false)
		{
			itemList.push_back(new ItemSlot());
			itemList.back()->AddToSlot(item, amount);
		}
	}
	else
	{
		itemList.push_back(new ItemSlot());
		itemList.back()->AddToSlot(item, amount);
	}
}

int ItemList :: AmountOfItemInInventory(int itemTemplateID) const
{
	for(std::vector<ItemSlot*>::const_iterator it = itemList.begin(); it != itemList.end(); it++)
		if( (*it)->GetTemplateID() == itemTemplateID)
			return (*it)->GetItemAmount();

	return 0;
}

void ItemList :: RemoveItemTypeFromInventory(int itemTemplateID, int amount)
{
	int slotIndex = 0;

	for(std::vector<ItemSlot*>::iterator it = itemList.begin(); it != itemList.end(); it++)
	{
		if( (*it)->GetTemplateID() == itemTemplateID && (*it)->GetItemAmount() >= amount)
			RemoveFromSlot(slotIndex, amount);

		slotIndex++;
	}
}

BaseItem* ItemList :: RemoveFromSlot(int index, int amount)
{
	BaseItem* item = itemList.at(index)->RemoveFromSlot(amount);

	if(itemList.at(index)->IsEmpty())
	{
		EraseSlot(index);
	}

	return item;
}

int ItemList :: GetSize() const
{
	return itemList.size();
}

ItemSlot* ItemList :: GetSlot(int index) const
{
	return itemList.at(index);
}

BaseItem* ItemList :: GetSlotItem(int index) const
{
	return itemList.at(index)->GetItem();
}

int ItemList :: GetSlotAmount(int index) const
{
	return itemList.at(index)->GetItemAmount();
}

void ItemList :: EraseSlot(int index)
{
	std::vector<ItemSlot*>::iterator it = itemList.begin();

	for(int i = 0; i < index; i++)
	{
		it++;
	}

	delete *it;
	itemList.erase(it);
}

void ItemList :: ClearSlot(int index)
{
	RemoveFromSlot(index, GetSlotAmount(index));
}

void ItemList :: ClearAll()
{
	for(std::vector<ItemSlot*>::iterator it = itemList.begin(); it != itemList.end(); it++)
		delete *it;

	itemList.clear();
}

void ItemList :: DebugDisplayContents()
{
	std::cout << "ItemList Contents: \n";

	for(int i = 0; i < GetSize(); i++)
	{
		std::cout << GetSlotItem(i)->GetName() << " - " << IRR->ToString(GetSlotAmount(i)) << "\n";
	}
}

float ItemList :: GetTotalWeight() const
{
	float totalWeight = 0;

	for(int i = 0; i < GetSize(); i++)
		totalWeight += GetSlotItem(i)->GetWeight() * GetSlotAmount(i);

	return totalWeight;
}
