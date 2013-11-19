#ifndef ITEM_SLOT_H
#define ITEM_SLOT_H

#include "IrrWrapper.h"
#include "BaseItem.h"

class ItemSlot
{
public:
	ItemSlot();
	~ItemSlot();

	bool	IsEmpty() const;

	void		AddToSlot(BaseItem* pItem, int setAmount); // Returns remaining;
	BaseItem*	RemoveFromSlot(int amount);
	BaseItem*	ClearSlot();
	bool		IsItemTemplateInSlot(int itemTemplateID, int amount);

	BaseItem*	GetItem();
	int			GetItemAmount() const;
	int 		GetTemplateID() const;
	std::string GetName() const;
	ItemType 	GetType() const;
	ITexture* 	GetInvTex();
	IMesh* 		GetMesh() const;
	int 		GetFuelValue() const;
	vector3df 	GetEquipPos() const;
	vector3df 	GetEquipRot() const;
	float 		GetEquipScale() const;
	bool 		GetIsStackable() const;
	bool 		GetIsUnique() const;
	float		GetWeight() const;

private:
	BaseItem* pItem;
	int itemAmount;
	static ITexture* emptySlotTexture;
};

#endif
