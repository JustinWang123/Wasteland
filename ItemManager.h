/*
	ItemTemplateID - Each template has a unique ID associated with it
	ItemID - Each item has an ID associated with it
		UniqueItem - have ItemIDs of 1001 or greater.  Multiple items can exist for each template
		Non-UniqueItem - have ItemIDs of 1000 or less.  Only a single item exists for each template
	ItemInstance - An item existing in the game world and drawn on screen as a 3d model.  Holds the ItemID of the item it represents
	ItemSlot - An item existing within a game instance such as in a characters inventory or inside of a chest

*/
#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include "BaseItemTemplate.h"
#include "BaseItem.h"
#include <map>

enum ItemNames
{
	// Tools:
	ITEM_SCRAP_SHOVEL = 0,
	ITEM_SCRAP_PICK,
	ITEM_SCRAP_METAL_CUTTERS,
	ITEM_SCRAP_SAW,

	// Projectile Weapons:
	ITEM_PISTOL,
	ITEM_RIFLE,
	ITEM_SHOTGUN,
	ITEM_MACHINE_GUN,

	// Ammunitions:
	ITEM_9_MM_BULLET,
	ITEM_762_BULLET,
	ITEM_SHOTGUN_SHELL,

	// Items:
	ITEM_WOOD_PLANK,
	ITEM_WOOD_STICK,
	ITEM_CONCRETE_DUST,
	ITEM_RUBBER,
	ITEM_SCRAP_METAL,
	ITEM_TIRE,
	ITEM_WORK_BENCH,
	ITEM_BARREL,
	ITEM_METAL_INGOT,
	ITEM_BARREL_FURNACE,
	ITEM_FIRE_HYDRANT,
	ITEM_STOP_SIGN,
	ITEM_METAL_POLE,
	ITEM_METAL_SHELF,
	ITEM_ANVIL,

	// Blocks:
	ITEM_WOOD_BLOCK,
	ITEM_METAL_BLOCK,
	ITEM_SCRAP_BLOCK,
	ITEM_CONCRETE_BLOCK,
	ITEM_SAND_BLOCK,
	ITEM_SCRAP_METAL_BLOCK,
	ITEM_ASPHALT_BLOCK,
	ITEM_BRICK_BLOCK,
	ITEM_GREEN_CONCRETE_BLOCK,
	ITEM_BLUE_CONCRETE_BLOCK,

	// Consumables:
	ITEM_MEDICAL_KIT,
	ITEM_WATER_BOTTLE,
	ITEM_CAN_FOOD,

	// Tire Armor:
	ITEM_TIRE_ARMOUR_HEAD,
	ITEM_TIRE_ARMOUR_CHEST,
	ITEM_TIRE_ARMOUR_LEG,
	ITEM_TIRE_ARMOUR_FEET,

	// Metal Armor:
	ITEM_METAL_ARMOUR_HEAD,
	ITEM_METAL_ARMOUR_CHEST,
	ITEM_METAL_ARMOUR_LEG,
	ITEM_METAL_ARMOUR_FEET
};

class ItemManager
{
public:
	static ItemManager* Instance();
	~ItemManager();

	void		Update();

	BaseItem*	CreateItem(int itemTemplateID);

	bool		ItemExists(int itemID);
	bool		ItemTemplateExists(int templateID);
	void		Register(BaseItem* i);
	BaseItem*	GetItem(int itemID) const;
	void		Remove(BaseItem* i);

	// Get template data:
	std::string GetName(int itemTemplateID)		{return itemTemplates[itemTemplateID]->name;}
	ItemType 	GetType(int itemTemplateID)		{return itemTemplates[itemTemplateID]->type;}
	ITexture* 	GetInvTex(int itemTemplateID)		{return itemTemplates[itemTemplateID]->invTex;}
	IMesh* 		GetMesh(int itemTemplateID)			{return itemTemplates[itemTemplateID]->mesh;}
	int 		GetFuelValue(int itemTemplateID)		{return itemTemplates[itemTemplateID]->fuelValue;}
	vector3df 	GetEquipPos(int itemTemplateID)	{return itemTemplates[itemTemplateID]->equipPos;}
	vector3df 	GetEquipRot(int itemTemplateID)	{return itemTemplates[itemTemplateID]->equipRot;}
	float 		GetEquipScale(int itemTemplateID)		{return itemTemplates[itemTemplateID]->equipScale;}
	bool 		GetIsStackable(int itemTemplateID)		{return itemTemplates[itemTemplateID]->isStackable;}
	bool 		GetIsUnique(int itemTemplateID)		{return itemTemplates[itemTemplateID]->isUnique;}
	float		GetWeight(int itemTemplateID)  {return itemTemplates[itemTemplateID]->weight;}

private:
	ItemManager();
	void InitItemTemplates();

	std::map<int, BaseItem*> items;
	std::map<int, BaseItemTemplate*> itemTemplates;

};

#define ItemMgr ItemManager::Instance()
#endif
