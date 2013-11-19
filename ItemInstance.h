#ifndef ITEM_INSTANCE_H
#define ITEM_INSTANCE_H

#include "BaseInstance.h"
#include "BaseItem.h"

class ItemInstance : public BaseInstance
{
public:
	ItemInstance(BaseItem* setItem, vector3df position, vector3df rotation);
	ItemInstance(BaseItem* setItem, int setAmount, vector3df position, vector3df rotation);
	~ItemInstance();

	virtual void Update();

	BaseItem*		GetItem() {return pItem;}
	int				GetItemAmount() {return itemAmount;}

	int 		GetItemID() const {return pItem->GetItemID();}
	int 		GetTemplateID() const {return pItem->GetTemplateID();}
	std::string GetName() const {return pItem->GetName();}
	ItemType 	GetType() const {return pItem->GetType();}
	ITexture* 	GetInvTex() const {return pItem->GetInvTex();}
	IMesh* 		GetMesh() const {return pItem->GetMesh();}
	int 		GetFuelValue() const {return pItem->GetFuelValue();}
	vector3df 	GetEquipPos() const {return pItem->GetEquipPos();}
	vector3df 	GetEquipRot() const {return pItem->GetEquipRot();}
	float 		GetEquipScale() const {return pItem->GetEquipScale();}
	bool 		GetIsStackable() const {return pItem->GetIsStackable();}
	bool 		GetIsUnique() const {return pItem->GetIsUnique();}
	float		GetWeight() const {return pItem->GetWeight();}

protected:
	void Init(BaseItem* setItem, int setAmount, vector3df position, vector3df rotation);
	BaseItem* pItem;
	int itemAmount;
	int instanceID;
};
#endif
