#include "ItemSlot.h"
#include "IrrWrapper.h"
#include "ItemManager.h"
#include "BaseItem.h"

ITexture* ItemSlot :: emptySlotTexture = 0;

ItemSlot :: ItemSlot()
{
	pItem = 0;
	itemAmount = 0;

	if(emptySlotTexture == 0)
	{
		emptySlotTexture = IRR->driver->getTexture("tex/InventorySlot.bmp");
	}
}

ItemSlot :: ~ItemSlot()
{
}

/*

void ItemSlot :: Draw(bool drawSlot, int x, int y)
{
	// Draw blank slot:
	if(drawSlot)
	{
		IRR->driver->draw2DImage(tex, position2d<s32>(x, y), rect<s32>(0,0,32,32), 0, SColor(255,255,255,255), true);
	}

	if(pItem != 0)
	{

		// Draw item texture:
		IRR->driver->draw2DImage(pItem->GetInvTex(), position2d<s32>(x, y), rect<s32>(0,0,32,32), 0, SColor(255,255,255,255), true);

		// If Item is stackable, draw amount:
		if(pItem->GetIsStackable())
			IRR->DrawText(IRR->ToString(itemAmount), x + 20, y, SColor(255, 0,0,0));


		// If item has durability, draw durability:
		if(GManager->itemData[itemID].maxDurability != 0)
		{
			float durabilityFraction = (float)durability / (float)GManager->itemData[itemID].maxDurability;
			int greenLength = (28.0 * durabilityFraction);
			int redLength = 28 - greenLength;

			// Draw Green line 2 thick:
			IRR->driver->draw2DLine(vector2di(x + 2, y + 28), vector2di(x + 2 + greenLength, y + 28), SColor(255,0,255,0));
			IRR->driver->draw2DLine(vector2di(x + 2, y + 29), vector2di(x + 2 + greenLength, y + 29), SColor(255,0,255,0));

			// Draw red line 2 thick:
			IRR->driver->draw2DLine(vector2di(x + 2 + greenLength, y + 28), vector2di(x + 2 + greenLength + redLength, y + 28), SColor(255,255,0,0));
			IRR->driver->draw2DLine(vector2di(x + 2 + greenLength, y + 29), vector2di(x + 2 + greenLength + redLength, y + 29), SColor(255,255,0,0));

		}

		// If item has ammo, draw ammo:
		if(GManager->itemData[itemID].maxAmmo != 0)
			IRR->DrawText(IRR->ToString(ammo), x + 2, y + 20, SColor(255, 255,0,0));

	}
}
*/





bool ItemSlot :: IsEmpty() const
{
	return pItem == 0 || itemAmount == 0;
}

void ItemSlot :: AddToSlot(BaseItem* setItem, int setAmount)
{
	// Add to existing stack:
	if(!IsEmpty() && pItem->GetTemplateID() == setItem->GetTemplateID() && pItem->GetIsStackable())
	{
		itemAmount += setAmount;
	}
	// Add to empty slot:
	else
	{
		pItem = setItem;
		itemAmount += setAmount;
	}
}

BaseItem* ItemSlot :: RemoveFromSlot(int amount)
{
	if(pItem != 0)
	{
		if(itemAmount > amount)
		{
			itemAmount -= amount;
		}
		else
		{
			itemAmount = 0;
			pItem = 0;
		}
	}

	return pItem;
}

BaseItem*  ItemSlot ::ClearSlot()
{
	BaseItem* temp = pItem;
	pItem = 0;
	itemAmount = 0;
	return temp;
}

bool ItemSlot :: IsItemTemplateInSlot(int templateID, int amount)
{
	return (pItem->GetTemplateID() == templateID && itemAmount >= amount);
}

ITexture* ItemSlot :: GetInvTex()
{
	if(!IsEmpty())
	{
		return  pItem->GetInvTex();
	}
	else
	{
		return emptySlotTexture;
	}
}

BaseItem*	ItemSlot :: GetItem()
{
	return pItem;
}

int	ItemSlot :: GetItemAmount() const
{
	return itemAmount;
}

int ItemSlot :: GetTemplateID() const
{
	if(!IsEmpty())
	{
		return pItem->GetTemplateID();
	}
	else
	{
		return 0;
	}

}

std::string ItemSlot :: GetName() const
{
	if(!IsEmpty())
	{
		return pItem->GetName();
	}
	else
	{
		return "Empty Slot";
	}

}

ItemType ItemSlot :: GetType() const
{
	if(!IsEmpty())
	{
		return pItem->GetType();
	}
	else
	{
		return ITEM_TYPE_BASE;
	}
}

IMesh* ItemSlot :: GetMesh() const
{
	if(!IsEmpty())
	{
		return pItem->GetMesh();
	}
	else
	{
		return 0;
	}
}

int ItemSlot :: GetFuelValue() const
{
	if(!IsEmpty())
	{
		return pItem->GetFuelValue();
	}
	else
	{
		return 0;
	}
}

vector3df ItemSlot :: GetEquipPos() const
{
	if(!IsEmpty())
	{
		return pItem->GetEquipPos();
	}
	else
	{
		return vector3df(0,0,0);
	}
}

vector3df ItemSlot :: GetEquipRot() const
{
	if(!IsEmpty())
	{
		return pItem->GetEquipRot();
	}
	else
	{
		return vector3df(0,0,0);
	}
}

float ItemSlot :: GetEquipScale() const
{
	if(!IsEmpty())
	{
		return pItem->GetEquipScale();
	}
	else
	{
		return 1.0f;
	}
}

bool ItemSlot :: GetIsStackable() const
{
	if(!IsEmpty())
	{
		return pItem->GetIsStackable();
	}
	else
	{
		return false;
	}
}

bool ItemSlot :: GetIsUnique() const
{
	if(!IsEmpty())
	{
		return pItem->GetIsUnique();
	}
	else
	{
		return false;
	}
}

float ItemSlot :: GetWeight() const
{
	if(!IsEmpty())
	{
		return pItem->GetWeight();
	}
	else
	{
		return 1.0f;
	}
}

