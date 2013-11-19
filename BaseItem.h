#ifndef BASE_ITEM_H
#define BASE_ITEM_H

#include "BaseItemTemplate.h"
class CharacterInstance;

class BaseItem
{
public:
	BaseItem(BaseItemTemplate* setTemplate); // For constructing unique items
	BaseItem(int setID, BaseItemTemplate* setTemplate); // For constructing nonunique items
	virtual ~BaseItem(){}

	virtual void Update() {}
	virtual void Use(CharacterInstance* user){}

	// Item Accessors:
	int GetItemID() const {return itemID;}

	// Template Accessors:
	int 		GetTemplateID() const {return pTemplate->id;}
	std::string GetName() const {return pTemplate->name;}
	ItemType 	GetType() const {return pTemplate->type;}
	ITexture* 	GetInvTex() const {return pTemplate->invTex;}
	IMesh* 		GetMesh() const {return pTemplate->mesh;}
	int 		GetFuelValue() const {return pTemplate->fuelValue;}
	vector3df 	GetEquipPos() const {return pTemplate->equipPos;}
	vector3df 	GetEquipRot() const {return pTemplate->equipRot;}
	float 		GetEquipScale() const {return pTemplate->equipScale;}
	bool 		GetIsStackable() const {return pTemplate->isStackable;}
	bool 		GetIsUnique() const {return pTemplate->isUnique;}
	float		GetWeight() const {return pTemplate->weight;}

protected:
	int itemID;
	static int nextID;

	BaseItemTemplate* pTemplate;
};
#endif
