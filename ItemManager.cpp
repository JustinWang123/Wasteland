#include "ItemManager.h"
#include "SimpleBlock.h"

/*
#include "BlockItemTemplate.h"
#include "ProjectileWeaponItemTemplate.h"
#include "ToolItemTemplate.h"
#include "ConsumableItemTemplate.h"
#include "ArmourItemTemplate.h"
#include "AmmoItemTemplate.h"
*/

#include "BaseItem.h"
#include "BlockItem.h"
#include "ProjectileWeaponItem.h"
#include "ToolItem.h"
#include "ConsumableItem.h"
#include "ArmourItem.h"
#include "ThrownWeaponItem.h"
#include "MeleeWeaponItem.h"
#include "AmmoItem.h"


#include <assert.h>

ItemManager* ItemManager :: Instance()
{
	static ItemManager instance;
	return &instance;
}

ItemManager :: ItemManager()
{
	InitItemTemplates();
}

ItemManager :: ~ItemManager()
{
	for(std::map<int, BaseItem*>::iterator it = items.begin(); it != items.end(); it++)
		delete (*it).second;

	for(std::map<int, BaseItemTemplate*>::iterator it = itemTemplates.begin(); it != itemTemplates.end(); it++)
		delete (*it).second;
}

void ItemManager :: Update()
{
	for(std::map<int, BaseItem*>::iterator it = items.begin(); it != items.end(); it++)
		(*it).second->Update();
}

void ItemManager :: InitItemTemplates()
{

	// Sand Block Item:
	itemTemplates[ITEM_SAND_BLOCK] = new BlockItemTemplate();
	itemTemplates[ITEM_SAND_BLOCK]->id = ITEM_SAND_BLOCK;
	itemTemplates[ITEM_SAND_BLOCK]->name = "Sand Block";
	itemTemplates[ITEM_SAND_BLOCK]->invTex = IRR->driver->getTexture("tex/Sand.bmp");
	itemTemplates[ITEM_SAND_BLOCK]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");
	itemTemplates[ITEM_SAND_BLOCK]->type = ITEM_TYPE_BLOCK;
	itemTemplates[ITEM_SAND_BLOCK]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_SAND_BLOCK]->equipScale = 0.05f;
	itemTemplates[ITEM_SAND_BLOCK]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_SAND_BLOCK]->fuelValue = 0;
	itemTemplates[ITEM_SAND_BLOCK]->isStackable = true;
	itemTemplates[ITEM_SAND_BLOCK]->isUnique = false;
	itemTemplates[ITEM_SAND_BLOCK]->weight = 0.5f;
	((BlockItemTemplate*)itemTemplates[ITEM_SAND_BLOCK])->buildBlockID = BLOCK_SAND;

	// Concrete Block Item:
	itemTemplates[ITEM_CONCRETE_BLOCK] = new BlockItemTemplate();
	itemTemplates[ITEM_CONCRETE_BLOCK]->id = ITEM_CONCRETE_BLOCK;
	itemTemplates[ITEM_CONCRETE_BLOCK]->name = "Concrete Block";
	itemTemplates[ITEM_CONCRETE_BLOCK]->invTex = IRR->driver->getTexture("tex/Stone.bmp");
	itemTemplates[ITEM_CONCRETE_BLOCK]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");;
	itemTemplates[ITEM_CONCRETE_BLOCK]->type = ITEM_TYPE_BLOCK;
	itemTemplates[ITEM_CONCRETE_BLOCK]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_CONCRETE_BLOCK]->equipScale = 0.05f;
	itemTemplates[ITEM_CONCRETE_BLOCK]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_CONCRETE_BLOCK]->fuelValue = 0;
	itemTemplates[ITEM_CONCRETE_BLOCK]->isStackable = true;
	itemTemplates[ITEM_CONCRETE_BLOCK]->isUnique = false;
	itemTemplates[ITEM_CONCRETE_BLOCK]->weight = 1.0f;
	((BlockItemTemplate*)itemTemplates[ITEM_CONCRETE_BLOCK])->buildBlockID = BLOCK_CONCRETE;

	// Metal Block Item:
	itemTemplates[ITEM_METAL_BLOCK] = new BlockItemTemplate();
	itemTemplates[ITEM_METAL_BLOCK]->id = ITEM_METAL_BLOCK;
	itemTemplates[ITEM_METAL_BLOCK]->name = "Metal Block";
	itemTemplates[ITEM_METAL_BLOCK]->invTex = IRR->driver->getTexture("tex/Metal.bmp");
	itemTemplates[ITEM_METAL_BLOCK]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");;
	itemTemplates[ITEM_METAL_BLOCK]->type = ITEM_TYPE_BLOCK;
	itemTemplates[ITEM_METAL_BLOCK]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_METAL_BLOCK]->equipScale = 0.05f;
	itemTemplates[ITEM_METAL_BLOCK]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_METAL_BLOCK]->fuelValue = 0;
	itemTemplates[ITEM_METAL_BLOCK]->isStackable = true;
	itemTemplates[ITEM_METAL_BLOCK]->isUnique = false;
	itemTemplates[ITEM_METAL_BLOCK]->weight = 1.0f;
	((BlockItemTemplate*)itemTemplates[ITEM_METAL_BLOCK])->buildBlockID = BLOCK_METAL;

	// Wood Block Item:
	itemTemplates[ITEM_WOOD_BLOCK] = new BlockItemTemplate();
	itemTemplates[ITEM_WOOD_BLOCK]->id = ITEM_WOOD_BLOCK;
	itemTemplates[ITEM_WOOD_BLOCK]->name = "Wood Block";
	itemTemplates[ITEM_WOOD_BLOCK]->invTex = IRR->driver->getTexture("tex/Wood.bmp");
	itemTemplates[ITEM_WOOD_BLOCK]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");;
	itemTemplates[ITEM_WOOD_BLOCK]->type = ITEM_TYPE_BLOCK;
	itemTemplates[ITEM_WOOD_BLOCK]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_WOOD_BLOCK]->equipScale = 0.05f;
	itemTemplates[ITEM_WOOD_BLOCK]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_WOOD_BLOCK]->fuelValue = 0;
	itemTemplates[ITEM_WOOD_BLOCK]->isStackable = true;
	itemTemplates[ITEM_WOOD_BLOCK]->isUnique = false;
	itemTemplates[ITEM_WOOD_BLOCK]->weight = 0.5f;
	((BlockItemTemplate*)itemTemplates[ITEM_WOOD_BLOCK])->buildBlockID = BLOCK_WOOD;

	// Scrap Metal Block Item:
	itemTemplates[ITEM_SCRAP_METAL_BLOCK] = new BlockItemTemplate();
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->id = ITEM_SCRAP_METAL_BLOCK;
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->name = "Scrap Metal Block";
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->invTex = IRR->driver->getTexture("tex/ScrapMetal.bmp");
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");;
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->type = ITEM_TYPE_BLOCK;
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->equipScale = 0.05f;
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->fuelValue = 0;
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->isStackable = true;
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->isUnique = false;
	itemTemplates[ITEM_SCRAP_METAL_BLOCK]->weight = 1.0f;
	((BlockItemTemplate*)itemTemplates[ITEM_SCRAP_METAL_BLOCK])->buildBlockID = BLOCK_SCRAP_METAL;

	// Asphalt block item:
	itemTemplates[ITEM_ASPHALT_BLOCK] = new BlockItemTemplate();
	itemTemplates[ITEM_ASPHALT_BLOCK]->id = ITEM_ASPHALT_BLOCK;
	itemTemplates[ITEM_ASPHALT_BLOCK]->name = "Asphalt Block";
	itemTemplates[ITEM_ASPHALT_BLOCK]->invTex = IRR->driver->getTexture("tex/Asphalt.bmp");
	itemTemplates[ITEM_ASPHALT_BLOCK]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");;
	itemTemplates[ITEM_ASPHALT_BLOCK]->type = ITEM_TYPE_BLOCK;
	itemTemplates[ITEM_ASPHALT_BLOCK]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_ASPHALT_BLOCK]->equipScale = 0.05f;
	itemTemplates[ITEM_ASPHALT_BLOCK]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_ASPHALT_BLOCK]->fuelValue = 0;
	itemTemplates[ITEM_ASPHALT_BLOCK]->isStackable = true;
	itemTemplates[ITEM_ASPHALT_BLOCK]->isUnique = false;
	itemTemplates[ITEM_ASPHALT_BLOCK]->weight = 1.0f;
	((BlockItemTemplate*)itemTemplates[ITEM_ASPHALT_BLOCK])->buildBlockID = BLOCK_ASPHALT;

	// Brick block item:
	itemTemplates[ITEM_BRICK_BLOCK] = new BlockItemTemplate();
	itemTemplates[ITEM_BRICK_BLOCK]->id = ITEM_BRICK_BLOCK;
	itemTemplates[ITEM_BRICK_BLOCK]->name = "Brick Block";
	itemTemplates[ITEM_BRICK_BLOCK]->invTex = IRR->driver->getTexture("tex/Brick.bmp");
	itemTemplates[ITEM_BRICK_BLOCK]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");;
	itemTemplates[ITEM_BRICK_BLOCK]->type = ITEM_TYPE_BLOCK;
	itemTemplates[ITEM_BRICK_BLOCK]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_BRICK_BLOCK]->equipScale = 0.05f;
	itemTemplates[ITEM_BRICK_BLOCK]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_BRICK_BLOCK]->fuelValue = 0;
	itemTemplates[ITEM_BRICK_BLOCK]->isStackable = true;
	itemTemplates[ITEM_BRICK_BLOCK]->isUnique = false;
	itemTemplates[ITEM_BRICK_BLOCK]->weight = 1.0f;
	((BlockItemTemplate*)itemTemplates[ITEM_BRICK_BLOCK])->buildBlockID = BLOCK_BRICK;

	// Green Concrete block item:
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK] = new BlockItemTemplate();
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->id = ITEM_GREEN_CONCRETE_BLOCK;
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->name = "Green Concrete Block";
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->invTex = IRR->driver->getTexture("tex/GreenConcrete.bmp");
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");;
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->type = ITEM_TYPE_BLOCK;
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->equipScale = 0.05f;
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->fuelValue = 0;
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->isStackable = true;
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->isUnique = false;
	itemTemplates[ITEM_GREEN_CONCRETE_BLOCK]->weight = 1.0f;
	((BlockItemTemplate*)itemTemplates[ITEM_GREEN_CONCRETE_BLOCK])->buildBlockID = BLOCK_GREEN_CONCRETE;

	// Blue Concrete block item:
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK] = new BlockItemTemplate();
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->id = ITEM_BLUE_CONCRETE_BLOCK;
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->name = "Brick Block";
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->invTex = IRR->driver->getTexture("tex/BlueConcrete.bmp");
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");;
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->type = ITEM_TYPE_BLOCK;
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->equipScale = 0.05f;
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->fuelValue = 0;
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->isStackable = true;
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->isUnique = false;
	itemTemplates[ITEM_BLUE_CONCRETE_BLOCK]->weight = 1.0f;
	((BlockItemTemplate*)itemTemplates[ITEM_BLUE_CONCRETE_BLOCK])->buildBlockID = BLOCK_BLUE_CONCRETE;

	// Scrap Shovel Item:
	itemTemplates[ITEM_SCRAP_SHOVEL] = new ToolItemTemplate();
	itemTemplates[ITEM_SCRAP_SHOVEL]->id = ITEM_SCRAP_SHOVEL;
	itemTemplates[ITEM_SCRAP_SHOVEL]->name = "Scrap Shovel";
	itemTemplates[ITEM_SCRAP_SHOVEL]->invTex = IRR->driver->getTexture("tex/Shovel.bmp");
	itemTemplates[ITEM_SCRAP_SHOVEL]->mesh = IRR->sceneMgr->getMesh("mdl/Shovel.irrmesh");
	itemTemplates[ITEM_SCRAP_SHOVEL]->type = ITEM_TYPE_TOOL;
	itemTemplates[ITEM_SCRAP_SHOVEL]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_SCRAP_SHOVEL]->equipScale = 0.1f;
	itemTemplates[ITEM_SCRAP_SHOVEL]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_SCRAP_SHOVEL]->fuelValue = 0;
	itemTemplates[ITEM_SCRAP_SHOVEL]->isStackable = false;
	itemTemplates[ITEM_SCRAP_SHOVEL]->isUnique = true;
	itemTemplates[ITEM_SCRAP_SHOVEL]->weight = 5.0f;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SHOVEL])->maxDurability = 100;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SHOVEL])->damage.type[DAMAGE_TYPE_FLESH] = 30;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SHOVEL])->damage.type[DAMAGE_TYPE_METAL] = 5;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SHOVEL])->damage.type[DAMAGE_TYPE_SOIL] = 30;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SHOVEL])->damage.type[DAMAGE_TYPE_STONE] = 5;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SHOVEL])->damage.type[DAMAGE_TYPE_WOOD] = 5;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SHOVEL])->delay = 0.5f;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SHOVEL])->range = 2.0f;

	// Scrap Pick Item:
	itemTemplates[ITEM_SCRAP_PICK] = new MeleeWeaponItemTemplate();
	itemTemplates[ITEM_SCRAP_PICK]->id = ITEM_SCRAP_PICK;
	itemTemplates[ITEM_SCRAP_PICK]->name = "Scrap Pick Axe";
	itemTemplates[ITEM_SCRAP_PICK]->invTex = IRR->driver->getTexture("tex/Pick.bmp");
	itemTemplates[ITEM_SCRAP_PICK]->mesh = IRR->sceneMgr->getMesh("mdl/PickAxe.irrmesh");
	itemTemplates[ITEM_SCRAP_PICK]->type = ITEM_TYPE_MELEE_WEAPON;
	itemTemplates[ITEM_SCRAP_PICK]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_SCRAP_PICK]->equipScale = 0.1f;
	itemTemplates[ITEM_SCRAP_PICK]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_SCRAP_PICK]->fuelValue = 0;
	itemTemplates[ITEM_SCRAP_PICK]->isStackable = false;
	itemTemplates[ITEM_SCRAP_PICK]->isUnique = true;
	itemTemplates[ITEM_SCRAP_PICK]->weight = 5.0f;
	((MeleeWeaponItemTemplate*)itemTemplates[ITEM_SCRAP_PICK])->damage.type[DAMAGE_TYPE_FLESH] = 30;
	((MeleeWeaponItemTemplate*)itemTemplates[ITEM_SCRAP_PICK])->damage.type[DAMAGE_TYPE_METAL] = 0;
	((MeleeWeaponItemTemplate*)itemTemplates[ITEM_SCRAP_PICK])->damage.type[DAMAGE_TYPE_SOIL] = 0;
	((MeleeWeaponItemTemplate*)itemTemplates[ITEM_SCRAP_PICK])->damage.type[DAMAGE_TYPE_STONE] = 0;
	((MeleeWeaponItemTemplate*)itemTemplates[ITEM_SCRAP_PICK])->damage.type[DAMAGE_TYPE_WOOD] = 0;
	((MeleeWeaponItemTemplate*)itemTemplates[ITEM_SCRAP_PICK])->delay = 0.5f;
	((MeleeWeaponItemTemplate*)itemTemplates[ITEM_SCRAP_PICK])->range = 2.0f;

	// Scrap Metal Cutter Item:
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS] = new ToolItemTemplate();
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->id = ITEM_SCRAP_METAL_CUTTERS;
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->name = "Scrap Metal Cutter";
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->invTex = IRR->driver->getTexture("tex/MetalCutter.bmp");
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->mesh = IRR->sceneMgr->getMesh("mdl/MetalCutter.irrmesh");
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->type = ITEM_TYPE_TOOL;
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->equipScale = 0.1f;
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->fuelValue = 0;
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->isStackable = false;
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->isUnique = true;
	itemTemplates[ITEM_SCRAP_METAL_CUTTERS]->weight = 5.0f;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_METAL_CUTTERS])->maxDurability = 100;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_METAL_CUTTERS])->damage.type[DAMAGE_TYPE_FLESH] = 30;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_METAL_CUTTERS])->damage.type[DAMAGE_TYPE_METAL] = 30;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_METAL_CUTTERS])->damage.type[DAMAGE_TYPE_SOIL] = 5;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_METAL_CUTTERS])->damage.type[DAMAGE_TYPE_STONE] = 5;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_METAL_CUTTERS])->damage.type[DAMAGE_TYPE_WOOD] = 5;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_METAL_CUTTERS])->delay = 0.5f;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_METAL_CUTTERS])->range = 2.0f;

	// Scrap Saw Item
	itemTemplates[ITEM_SCRAP_SAW] = new ToolItemTemplate();
	itemTemplates[ITEM_SCRAP_SAW]->id = ITEM_SCRAP_SAW;
	itemTemplates[ITEM_SCRAP_SAW]->name = "Saw";
	itemTemplates[ITEM_SCRAP_SAW]->invTex = IRR->driver->getTexture("tex/Saw.bmp");
	itemTemplates[ITEM_SCRAP_SAW]->mesh = IRR->sceneMgr->getMesh("mdl/Saw.irrmesh");
	itemTemplates[ITEM_SCRAP_SAW]->type = ITEM_TYPE_TOOL;
	itemTemplates[ITEM_SCRAP_SAW]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_SCRAP_SAW]->equipScale = 0.1f;
	itemTemplates[ITEM_SCRAP_SAW]->equipRot = vector3df(90,-90,0);
	itemTemplates[ITEM_SCRAP_SAW]->fuelValue = 0;
	itemTemplates[ITEM_SCRAP_SAW]->isStackable = false;
	itemTemplates[ITEM_SCRAP_SAW]->isUnique = true;
	itemTemplates[ITEM_SCRAP_SAW]->weight = 5.0f;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SAW])->maxDurability = 100;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SAW])->damage.type[DAMAGE_TYPE_FLESH] = 30;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SAW])->damage.type[DAMAGE_TYPE_METAL] = 5;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SAW])->damage.type[DAMAGE_TYPE_SOIL] = 5;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SAW])->damage.type[DAMAGE_TYPE_STONE] = 5;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SAW])->damage.type[DAMAGE_TYPE_WOOD] = 30;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SAW])->delay = 0.5f;
	((ToolItemTemplate*)itemTemplates[ITEM_SCRAP_SAW])->range = 2.0f;



	// RifleItem:
	itemTemplates[ITEM_RIFLE] = new ProjectileWeaponItemTemplate();
	itemTemplates[ITEM_RIFLE]->id = ITEM_RIFLE;
	itemTemplates[ITEM_RIFLE]->name = "Rifle";
	itemTemplates[ITEM_RIFLE]->invTex = IRR->driver->getTexture("tex/Rifle.bmp");
	itemTemplates[ITEM_RIFLE]->mesh = IRR->sceneMgr->getMesh("mdl/Rifle.irrmesh");
	itemTemplates[ITEM_RIFLE]->type = ITEM_TYPE_PROJECTILE_WEAPON;
	itemTemplates[ITEM_RIFLE]->equipPos = vector3df(0.1f,-0.1f,0.3f);
	itemTemplates[ITEM_RIFLE]->equipScale = 0.1f;
	itemTemplates[ITEM_RIFLE]->equipRot = vector3df(0,180,90);
	itemTemplates[ITEM_RIFLE]->fuelValue = 0;
	itemTemplates[ITEM_RIFLE]->isStackable = false;
	itemTemplates[ITEM_RIFLE]->isUnique = true;
	itemTemplates[ITEM_RIFLE]->weight = 8.0f;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_RIFLE])->maxDurability = 100;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_RIFLE])->ammoItemTemplateID = ITEM_762_BULLET;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_RIFLE])->cartridgeType = CARTRIDGE_TYPE_SLUG;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_RIFLE])->clipSize = 8;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_RIFLE])->fireMode = FIRE_MODE_BURST;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_RIFLE])->reloadTime = 5.0f;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_RIFLE])->shootDelay = 1.0f;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_RIFLE])->range = 30;


	// Pistol:
	itemTemplates[ITEM_PISTOL] = new ProjectileWeaponItemTemplate();
	itemTemplates[ITEM_PISTOL]->id = ITEM_PISTOL;
	itemTemplates[ITEM_PISTOL]->name = "Pistol";
	itemTemplates[ITEM_PISTOL]->invTex = IRR->driver->getTexture("tex/Pistol.bmp");
	itemTemplates[ITEM_PISTOL]->mesh = IRR->sceneMgr->getMesh("mdl/Pistol.irrmesh");
	itemTemplates[ITEM_PISTOL]->type = ITEM_TYPE_PROJECTILE_WEAPON;
	itemTemplates[ITEM_PISTOL]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_PISTOL]->equipScale = 0.2f;
	itemTemplates[ITEM_PISTOL]->equipRot = vector3df(0,180,90);
	itemTemplates[ITEM_PISTOL]->fuelValue = 0;
	itemTemplates[ITEM_PISTOL]->isStackable = false;
	itemTemplates[ITEM_PISTOL]->isUnique = true;
	itemTemplates[ITEM_PISTOL]->weight = 4.0f;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_PISTOL])->maxDurability = 100;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_PISTOL])->ammoItemTemplateID = ITEM_9_MM_BULLET;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_PISTOL])->cartridgeType = CARTRIDGE_TYPE_SLUG;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_PISTOL])->clipSize = 8;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_PISTOL])->fireMode = FIRE_MODE_SEMI_AUTO;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_PISTOL])->reloadTime = 5.0f;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_PISTOL])->shootDelay = 0.5f;
    ((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_PISTOL])->range = 15;

	// Shotgun:
	itemTemplates[ITEM_SHOTGUN] = new ProjectileWeaponItemTemplate();
	itemTemplates[ITEM_SHOTGUN]->id = ITEM_SHOTGUN;
	itemTemplates[ITEM_SHOTGUN]->name = "Shot Gun";
	itemTemplates[ITEM_SHOTGUN]->invTex = IRR->driver->getTexture("tex/Shotgun.bmp");
	itemTemplates[ITEM_SHOTGUN]->mesh = IRR->sceneMgr->getMesh("mdl/Rifle.irrmesh");
	itemTemplates[ITEM_SHOTGUN]->type = ITEM_TYPE_PROJECTILE_WEAPON;
	itemTemplates[ITEM_SHOTGUN]->equipPos = vector3df(0.1f,-0.1f,0.3f);
	itemTemplates[ITEM_SHOTGUN]->equipScale = 0.1f;
	itemTemplates[ITEM_SHOTGUN]->equipRot = vector3df(0,180,90);
	itemTemplates[ITEM_SHOTGUN]->fuelValue = 0;
	itemTemplates[ITEM_SHOTGUN]->isStackable = false;
	itemTemplates[ITEM_SHOTGUN]->isUnique = true;
	itemTemplates[ITEM_SHOTGUN]->weight = 8.0f;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_SHOTGUN])->maxDurability = 100;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_SHOTGUN])->ammoItemTemplateID = ITEM_SHOTGUN_SHELL;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_SHOTGUN])->cartridgeType = CARTRIDGE_TYPE_SHOT;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_SHOTGUN])->clipSize = 2;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_SHOTGUN])->fireMode = FIRE_MODE_SEMI_AUTO;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_SHOTGUN])->reloadTime = 5.0f;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_SHOTGUN])->shootDelay = 1.0f;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_SHOTGUN])->range = 15;

	// Machine Gun Item:
	itemTemplates[ITEM_MACHINE_GUN] = new ProjectileWeaponItemTemplate();
	itemTemplates[ITEM_MACHINE_GUN]->id = ITEM_MACHINE_GUN;
	itemTemplates[ITEM_MACHINE_GUN]->name = "Machine Gun";
	itemTemplates[ITEM_MACHINE_GUN]->invTex = IRR->driver->getTexture("tex/Machine Gun.bmp");
	itemTemplates[ITEM_MACHINE_GUN]->mesh = IRR->sceneMgr->getMesh("mdl/Rifle.irrmesh");
	itemTemplates[ITEM_MACHINE_GUN]->type = ITEM_TYPE_PROJECTILE_WEAPON;
	itemTemplates[ITEM_MACHINE_GUN]->equipPos = vector3df(0.1f,-0.1f,0.3f);
	itemTemplates[ITEM_MACHINE_GUN]->equipScale = 0.1f;
	itemTemplates[ITEM_MACHINE_GUN]->equipRot = vector3df(0,180,90);
	itemTemplates[ITEM_MACHINE_GUN]->fuelValue = 0;
	itemTemplates[ITEM_MACHINE_GUN]->isStackable = false;
	itemTemplates[ITEM_MACHINE_GUN]->isUnique = true;
	itemTemplates[ITEM_MACHINE_GUN]->weight = 12.0f;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_MACHINE_GUN])->maxDurability = 100;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_MACHINE_GUN])->ammoItemTemplateID = ITEM_762_BULLET;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_MACHINE_GUN])->cartridgeType = CARTRIDGE_TYPE_SLUG;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_MACHINE_GUN])->clipSize = 60;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_MACHINE_GUN])->fireMode = FIRE_MODE_BURST;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_MACHINE_GUN])->reloadTime = 10.0f;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_MACHINE_GUN])->shootDelay = 0.2f;
	((ProjectileWeaponItemTemplate*)itemTemplates[ITEM_MACHINE_GUN])->range = 20;


	// 9mm Bullet Item
	itemTemplates[ITEM_9_MM_BULLET] = new AmmoItemTemplate();
	itemTemplates[ITEM_9_MM_BULLET]->id = ITEM_9_MM_BULLET;
	itemTemplates[ITEM_9_MM_BULLET]->name = "9mm Bullet";
	itemTemplates[ITEM_9_MM_BULLET]->invTex = IRR->driver->getTexture("tex/9mm Bullet.bmp");
	itemTemplates[ITEM_9_MM_BULLET]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");
	itemTemplates[ITEM_9_MM_BULLET]->type = ITEM_TYPE_AMMO;
	itemTemplates[ITEM_9_MM_BULLET]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_9_MM_BULLET]->equipScale = 0.1f;
	itemTemplates[ITEM_9_MM_BULLET]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_9_MM_BULLET]->fuelValue = 10;
	itemTemplates[ITEM_9_MM_BULLET]->isStackable = true;
	itemTemplates[ITEM_9_MM_BULLET]->isUnique = false;
	itemTemplates[ITEM_9_MM_BULLET]->weight = 0.1f;
	((AmmoItemTemplate*)itemTemplates[ITEM_9_MM_BULLET])->damage.type[DAMAGE_TYPE_FLESH] = 1; // 25
	((AmmoItemTemplate*)itemTemplates[ITEM_9_MM_BULLET])->damage.type[DAMAGE_TYPE_METAL] = 0;
	((AmmoItemTemplate*)itemTemplates[ITEM_9_MM_BULLET])->damage.type[DAMAGE_TYPE_SOIL] = 0;
	((AmmoItemTemplate*)itemTemplates[ITEM_9_MM_BULLET])->damage.type[DAMAGE_TYPE_STONE] = 0;
	((AmmoItemTemplate*)itemTemplates[ITEM_9_MM_BULLET])->damage.type[DAMAGE_TYPE_WOOD] = 0;

	// 7.62 Bullet Item
	itemTemplates[ITEM_762_BULLET] = new AmmoItemTemplate();
	itemTemplates[ITEM_762_BULLET]->id = ITEM_762_BULLET;
	itemTemplates[ITEM_762_BULLET]->name = "7.62 Bullet";
	itemTemplates[ITEM_762_BULLET]->invTex = IRR->driver->getTexture("tex/7.62 Bullet.bmp");
	itemTemplates[ITEM_762_BULLET]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");
	itemTemplates[ITEM_762_BULLET]->type = ITEM_TYPE_AMMO;
	itemTemplates[ITEM_762_BULLET]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_762_BULLET]->equipScale = 0.1f;
	itemTemplates[ITEM_762_BULLET]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_762_BULLET]->fuelValue = 10;
	itemTemplates[ITEM_762_BULLET]->isStackable = true;
	itemTemplates[ITEM_762_BULLET]->isUnique = false;
	itemTemplates[ITEM_762_BULLET]->weight = 0.1f;
	((AmmoItemTemplate*)itemTemplates[ITEM_762_BULLET])->damage.type[DAMAGE_TYPE_FLESH] = 1; // 25
	((AmmoItemTemplate*)itemTemplates[ITEM_762_BULLET])->damage.type[DAMAGE_TYPE_METAL] = 0;
	((AmmoItemTemplate*)itemTemplates[ITEM_762_BULLET])->damage.type[DAMAGE_TYPE_SOIL] = 0;
	((AmmoItemTemplate*)itemTemplates[ITEM_762_BULLET])->damage.type[DAMAGE_TYPE_STONE] = 0;
	((AmmoItemTemplate*)itemTemplates[ITEM_762_BULLET])->damage.type[DAMAGE_TYPE_WOOD] = 0;

	// Shotgun Shell Item
	itemTemplates[ITEM_SHOTGUN_SHELL] = new AmmoItemTemplate();
	itemTemplates[ITEM_SHOTGUN_SHELL]->id = ITEM_SHOTGUN_SHELL;
	itemTemplates[ITEM_SHOTGUN_SHELL]->name = "Shotgun Shell";
	itemTemplates[ITEM_SHOTGUN_SHELL]->invTex = IRR->driver->getTexture("tex/Shotgun Shell.bmp");
	itemTemplates[ITEM_SHOTGUN_SHELL]->mesh = IRR->sceneMgr->getMesh("mdl/Block.irrmesh");
	itemTemplates[ITEM_SHOTGUN_SHELL]->type = ITEM_TYPE_AMMO;
	itemTemplates[ITEM_SHOTGUN_SHELL]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_SHOTGUN_SHELL]->equipScale = 0.1f;
	itemTemplates[ITEM_SHOTGUN_SHELL]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_SHOTGUN_SHELL]->fuelValue = 10;
	itemTemplates[ITEM_SHOTGUN_SHELL]->isStackable = true;
	itemTemplates[ITEM_SHOTGUN_SHELL]->isUnique = false;
	itemTemplates[ITEM_SHOTGUN_SHELL]->weight = 0.1f;
	((AmmoItemTemplate*)itemTemplates[ITEM_SHOTGUN_SHELL])->damage.type[DAMAGE_TYPE_FLESH] = 1; // 25
	((AmmoItemTemplate*)itemTemplates[ITEM_SHOTGUN_SHELL])->damage.type[DAMAGE_TYPE_METAL] = 0;
	((AmmoItemTemplate*)itemTemplates[ITEM_SHOTGUN_SHELL])->damage.type[DAMAGE_TYPE_SOIL] = 0;
	((AmmoItemTemplate*)itemTemplates[ITEM_SHOTGUN_SHELL])->damage.type[DAMAGE_TYPE_STONE] = 0;
	((AmmoItemTemplate*)itemTemplates[ITEM_SHOTGUN_SHELL])->damage.type[DAMAGE_TYPE_WOOD] = 0;



	// Tire Item
	itemTemplates[ITEM_TIRE] = new BaseItemTemplate();
	itemTemplates[ITEM_TIRE]->id = ITEM_TIRE;
	itemTemplates[ITEM_TIRE]->name = "Tire";
	itemTemplates[ITEM_TIRE]->invTex = IRR->driver->getTexture("tex/Tire.bmp");
	itemTemplates[ITEM_TIRE]->mesh = IRR->sceneMgr->getMesh("mdl/Tire.irrmesh");
	itemTemplates[ITEM_TIRE]->type = ITEM_TYPE_BASE;
	itemTemplates[ITEM_TIRE]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_TIRE]->equipScale = 0.1f;
	itemTemplates[ITEM_TIRE]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_TIRE]->fuelValue = 10;
	itemTemplates[ITEM_TIRE]->isStackable = true;
	itemTemplates[ITEM_TIRE]->isUnique = false;
	itemTemplates[ITEM_TIRE]->weight = 2.0f;

	// Work Bench Item
	itemTemplates[ITEM_WORK_BENCH] = new BaseItemTemplate();
	itemTemplates[ITEM_WORK_BENCH]->id = ITEM_WORK_BENCH;
	itemTemplates[ITEM_WORK_BENCH]->name = "Work Bench";
	itemTemplates[ITEM_WORK_BENCH]->invTex = IRR->driver->getTexture("tex/WorkBench.bmp");
	itemTemplates[ITEM_WORK_BENCH]->mesh = IRR->sceneMgr->getMesh("mdl/WorkBench.irrmesh");
	itemTemplates[ITEM_WORK_BENCH]->type = ITEM_TYPE_BASE;
	itemTemplates[ITEM_WORK_BENCH]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_WORK_BENCH]->equipScale = 0.02f;
	itemTemplates[ITEM_WORK_BENCH]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_WORK_BENCH]->fuelValue = 0;
	itemTemplates[ITEM_WORK_BENCH]->isStackable = false;
	itemTemplates[ITEM_WORK_BENCH]->isUnique = false;
	itemTemplates[ITEM_WORK_BENCH]->weight = 5.0f;

	// Barrel Item
	itemTemplates[ITEM_BARREL] = new BaseItemTemplate();
	itemTemplates[ITEM_BARREL]->id = ITEM_BARREL;
	itemTemplates[ITEM_BARREL]->name = "Barrel";
	itemTemplates[ITEM_BARREL]->invTex = IRR->driver->getTexture("tex/Barrel.bmp");
	itemTemplates[ITEM_BARREL]->mesh = IRR->sceneMgr->getMesh("mdl/Barrel.irrmesh");
	itemTemplates[ITEM_BARREL]->type = ITEM_TYPE_BASE;
	itemTemplates[ITEM_BARREL]->equipPos = vector3df(0.05f, -0.05f, 0.08f);
	itemTemplates[ITEM_BARREL]->equipScale = 0.02f;
	itemTemplates[ITEM_BARREL]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_BARREL]->fuelValue = 0;
	itemTemplates[ITEM_BARREL]->isStackable = false;
	itemTemplates[ITEM_BARREL]->isUnique = false;
	itemTemplates[ITEM_BARREL]->weight = 5.0f;

	// Barrel Furnace Item
	itemTemplates[ITEM_BARREL_FURNACE] = new BaseItemTemplate();
	itemTemplates[ITEM_BARREL_FURNACE]->id = ITEM_BARREL_FURNACE;
	itemTemplates[ITEM_BARREL_FURNACE]->name = "Barrel Furnace";
	itemTemplates[ITEM_BARREL_FURNACE]->invTex = IRR->driver->getTexture("tex/BarrelFurnace.bmp");
	itemTemplates[ITEM_BARREL_FURNACE]->mesh = IRR->sceneMgr->getMesh("mdl/BarrelFurnace.irrmesh");
	itemTemplates[ITEM_BARREL_FURNACE]->type = ITEM_TYPE_BASE;
	itemTemplates[ITEM_BARREL_FURNACE]->equipPos = vector3df(0.05f, -0.05f, 0.08f);
	itemTemplates[ITEM_BARREL_FURNACE]->equipScale = 0.02f;
	itemTemplates[ITEM_BARREL_FURNACE]->equipRot = vector3df(0,30,0);
	itemTemplates[ITEM_BARREL_FURNACE]->fuelValue = 0;
	itemTemplates[ITEM_BARREL_FURNACE]->isStackable = false;
	itemTemplates[ITEM_BARREL_FURNACE]->isUnique = false;
	itemTemplates[ITEM_BARREL_FURNACE]->weight = 5.0f;

	// Anvil:
	itemTemplates[ITEM_ANVIL] = new BaseItemTemplate();
	itemTemplates[ITEM_ANVIL]->id = ITEM_ANVIL;
	itemTemplates[ITEM_ANVIL]->name = "Anvil";
	itemTemplates[ITEM_ANVIL]->invTex = IRR->driver->getTexture("tex/Anvil.bmp");
	itemTemplates[ITEM_ANVIL]->mesh = IRR->sceneMgr->getMesh("mdl/Anvil.irrmesh");
	itemTemplates[ITEM_ANVIL]->type = ITEM_TYPE_BASE;
	itemTemplates[ITEM_ANVIL]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_ANVIL]->equipScale = 0.1f;
	itemTemplates[ITEM_ANVIL]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_ANVIL]->fuelValue = 0;
	itemTemplates[ITEM_ANVIL]->isStackable = false;
	itemTemplates[ITEM_ANVIL]->isUnique = false;
	itemTemplates[ITEM_ANVIL]->weight = 30.0f;

	// Metal Ingot Item
	itemTemplates[ITEM_METAL_INGOT] = new BaseItemTemplate();
	itemTemplates[ITEM_METAL_INGOT]->id = ITEM_METAL_INGOT;
	itemTemplates[ITEM_METAL_INGOT]->name = "Metal Ingot";
	itemTemplates[ITEM_METAL_INGOT]->invTex = IRR->driver->getTexture("tex/MetalIngot.bmp");
	itemTemplates[ITEM_METAL_INGOT]->mesh = IRR->sceneMgr->getMesh("mdl/MetalIngot.irrmesh");
	itemTemplates[ITEM_METAL_INGOT]->type = ITEM_TYPE_BASE;
	itemTemplates[ITEM_METAL_INGOT]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_METAL_INGOT]->equipScale = 0.05f;
	itemTemplates[ITEM_METAL_INGOT]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_METAL_INGOT]->fuelValue = 0;
	itemTemplates[ITEM_METAL_INGOT]->isStackable = true;
	itemTemplates[ITEM_METAL_INGOT]->isUnique = false;
	itemTemplates[ITEM_METAL_INGOT]->weight = 3.0f;

	// Fire Hydrant Item
	itemTemplates[ITEM_FIRE_HYDRANT] = new BaseItemTemplate();
	itemTemplates[ITEM_FIRE_HYDRANT]->id = ITEM_FIRE_HYDRANT;
	itemTemplates[ITEM_FIRE_HYDRANT]->name = "Fire Hydrant";
	itemTemplates[ITEM_FIRE_HYDRANT]->invTex = IRR->driver->getTexture("tex/FireHydrant.bmp");
	itemTemplates[ITEM_FIRE_HYDRANT]->mesh = IRR->sceneMgr->getMesh("mdl/FireHydrant.irrmesh");
	itemTemplates[ITEM_FIRE_HYDRANT]->type = ITEM_TYPE_BASE;
	itemTemplates[ITEM_FIRE_HYDRANT]->equipPos = vector3df(0.05f, -0.06f, 0.08f);
	itemTemplates[ITEM_FIRE_HYDRANT]->equipScale = 0.03f;
	itemTemplates[ITEM_FIRE_HYDRANT]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_FIRE_HYDRANT]->fuelValue = 0;
	itemTemplates[ITEM_FIRE_HYDRANT]->isStackable = false;
	itemTemplates[ITEM_FIRE_HYDRANT]->isUnique = false;
	itemTemplates[ITEM_FIRE_HYDRANT]->weight = 20.0f;

	// Stop Sign Item
	itemTemplates[ITEM_STOP_SIGN] = new BaseItemTemplate();
	itemTemplates[ITEM_STOP_SIGN]->id = ITEM_STOP_SIGN;
	itemTemplates[ITEM_STOP_SIGN]->name = "Stop Sign";
	itemTemplates[ITEM_STOP_SIGN]->invTex = IRR->driver->getTexture("tex/StopSign.bmp");
	itemTemplates[ITEM_STOP_SIGN]->mesh = IRR->sceneMgr->getMesh("mdl/StopSign.irrmesh");
	itemTemplates[ITEM_STOP_SIGN]->type = ITEM_TYPE_BASE;
	itemTemplates[ITEM_STOP_SIGN]->equipPos = vector3df(0.05f, -0.1f, 0.08f);
	itemTemplates[ITEM_STOP_SIGN]->equipScale = 0.03f;
	itemTemplates[ITEM_STOP_SIGN]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_STOP_SIGN]->fuelValue = 0;
	itemTemplates[ITEM_STOP_SIGN]->isStackable = false;
	itemTemplates[ITEM_STOP_SIGN]->isUnique = false;
	itemTemplates[ITEM_STOP_SIGN]->weight = 20.0f;

	// Metal Pole
	itemTemplates[ITEM_METAL_POLE] = new BaseItemTemplate();
	itemTemplates[ITEM_METAL_POLE]->id = ITEM_METAL_POLE;
	itemTemplates[ITEM_METAL_POLE]->name = "Metal Pole";
	itemTemplates[ITEM_METAL_POLE]->invTex = IRR->driver->getTexture("tex/MetalPole.bmp");
	itemTemplates[ITEM_METAL_POLE]->mesh = IRR->sceneMgr->getMesh("mdl/MetalPole.irrmesh");
	itemTemplates[ITEM_METAL_POLE]->type = ITEM_TYPE_BASE;
	itemTemplates[ITEM_METAL_POLE]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_METAL_POLE]->equipScale = 0.05f;
	itemTemplates[ITEM_METAL_POLE]->equipRot = vector3df(90,-10,0);
	itemTemplates[ITEM_METAL_POLE]->fuelValue = 0;
	itemTemplates[ITEM_METAL_POLE]->isStackable = true;
	itemTemplates[ITEM_METAL_POLE]->isUnique = false;
	itemTemplates[ITEM_METAL_POLE]->weight = 10.0f;

	// Metal Shelf
	itemTemplates[ITEM_METAL_SHELF] = new BaseItemTemplate();
	itemTemplates[ITEM_METAL_SHELF]->id = ITEM_METAL_SHELF;
	itemTemplates[ITEM_METAL_SHELF]->name = "Metal Shelf";
	itemTemplates[ITEM_METAL_SHELF]->invTex = IRR->driver->getTexture("tex/MetalShelf.bmp");
	itemTemplates[ITEM_METAL_SHELF]->mesh = IRR->sceneMgr->getMesh("mdl/MetalShelf.irrmesh");
	itemTemplates[ITEM_METAL_SHELF]->type = ITEM_TYPE_BASE;
	itemTemplates[ITEM_METAL_SHELF]->equipPos = vector3df(0.1f, -0.1f, 0.08f);
	itemTemplates[ITEM_METAL_SHELF]->equipScale = 0.03f;
	itemTemplates[ITEM_METAL_SHELF]->equipRot = vector3df(0,70,0);
	itemTemplates[ITEM_METAL_SHELF]->fuelValue = 0;
	itemTemplates[ITEM_METAL_SHELF]->isStackable = false;
	itemTemplates[ITEM_METAL_SHELF]->isUnique = false;
	itemTemplates[ITEM_METAL_SHELF]->weight = 30.0f;

	// Medical Kit
	itemTemplates[ITEM_MEDICAL_KIT] = new ConsumableItemTemplate();
	itemTemplates[ITEM_MEDICAL_KIT]->id = ITEM_MEDICAL_KIT;
	itemTemplates[ITEM_MEDICAL_KIT]->name = "Medical Kit";
	itemTemplates[ITEM_MEDICAL_KIT]->invTex = IRR->driver->getTexture("tex/MedicalKit.bmp");
	itemTemplates[ITEM_MEDICAL_KIT]->mesh = IRR->sceneMgr->getMesh("mdl/MedicalKit.irrmesh");
	itemTemplates[ITEM_MEDICAL_KIT]->type = ITEM_TYPE_CONSUMABLE;
	itemTemplates[ITEM_MEDICAL_KIT]->equipPos = vector3df(0.06f, -0.04f, 0.08f);
	itemTemplates[ITEM_MEDICAL_KIT]->equipScale = 0.03f;
	itemTemplates[ITEM_MEDICAL_KIT]->equipRot = vector3df(0,70,0);
	itemTemplates[ITEM_MEDICAL_KIT]->fuelValue = 0;
	itemTemplates[ITEM_MEDICAL_KIT]->isStackable = true;
	itemTemplates[ITEM_MEDICAL_KIT]->isUnique = false;
	itemTemplates[ITEM_MEDICAL_KIT]->weight = 3.0f;
	((ConsumableItemTemplate*)itemTemplates[ITEM_MEDICAL_KIT])->health = 50;
	((ConsumableItemTemplate*)itemTemplates[ITEM_MEDICAL_KIT])->hunger = 0;
	((ConsumableItemTemplate*)itemTemplates[ITEM_MEDICAL_KIT])->thirst = 0;

	// Food Can Item
	itemTemplates[ITEM_CAN_FOOD] = new ConsumableItemTemplate();
	itemTemplates[ITEM_CAN_FOOD]->id = ITEM_CAN_FOOD;
	itemTemplates[ITEM_CAN_FOOD]->name = "Food Can";
	itemTemplates[ITEM_CAN_FOOD]->invTex = IRR->driver->getTexture("tex/FoodCan.bmp");
	itemTemplates[ITEM_CAN_FOOD]->mesh = IRR->sceneMgr->getMesh("mdl/FoodCan.irrmesh");
	itemTemplates[ITEM_CAN_FOOD]->type = ITEM_TYPE_CONSUMABLE;
	itemTemplates[ITEM_CAN_FOOD]->equipPos = vector3df(0.05f, -0.04f, 0.08f);
	itemTemplates[ITEM_CAN_FOOD]->equipScale = 0.05f;
	itemTemplates[ITEM_CAN_FOOD]->equipRot = vector3df(0,-10,0);
	itemTemplates[ITEM_CAN_FOOD]->fuelValue = 0;
	itemTemplates[ITEM_CAN_FOOD]->isStackable = true;
	itemTemplates[ITEM_CAN_FOOD]->isUnique = false;
	itemTemplates[ITEM_CAN_FOOD]->weight = 1.0f;
	((ConsumableItemTemplate*)itemTemplates[ITEM_CAN_FOOD])->health = 0;
	((ConsumableItemTemplate*)itemTemplates[ITEM_CAN_FOOD])->hunger = 50;
	((ConsumableItemTemplate*)itemTemplates[ITEM_CAN_FOOD])->thirst = 0;

	// Water Bottle:
	itemTemplates[ITEM_WATER_BOTTLE] = new ConsumableItemTemplate();
	itemTemplates[ITEM_WATER_BOTTLE]->id = ITEM_WATER_BOTTLE;
	itemTemplates[ITEM_WATER_BOTTLE]->name = "Water Bottle";
	itemTemplates[ITEM_WATER_BOTTLE]->invTex = IRR->driver->getTexture("tex/WaterBottle.bmp");
	itemTemplates[ITEM_WATER_BOTTLE]->mesh = IRR->sceneMgr->getMesh("mdl/WaterBottle.irrmesh");
	itemTemplates[ITEM_WATER_BOTTLE]->type = ITEM_TYPE_CONSUMABLE;
	itemTemplates[ITEM_WATER_BOTTLE]->equipPos = vector3df(0.2f,-0.2f,0.3f);
	itemTemplates[ITEM_WATER_BOTTLE]->equipScale = 0.15f;
	itemTemplates[ITEM_WATER_BOTTLE]->equipRot = vector3df(0,90,0);
	itemTemplates[ITEM_WATER_BOTTLE]->fuelValue = 0;
	itemTemplates[ITEM_WATER_BOTTLE]->isStackable = true;
	itemTemplates[ITEM_WATER_BOTTLE]->isUnique = false;
	itemTemplates[ITEM_WATER_BOTTLE]->weight = 1.0f;
	((ConsumableItemTemplate*)itemTemplates[ITEM_WATER_BOTTLE])->health = 0;
	((ConsumableItemTemplate*)itemTemplates[ITEM_WATER_BOTTLE])->hunger = 0;
	((ConsumableItemTemplate*)itemTemplates[ITEM_WATER_BOTTLE])->thirst = 50;

	// Tire Helm:
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD] = new ArmourItemTemplate();
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->id = ITEM_TIRE_ARMOUR_HEAD;
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->name = "Tire Helm";
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->invTex = IRR->driver->getTexture("tex/TireArmourHead.bmp");
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->mesh = IRR->sceneMgr->getMesh("mdl/Tire.irrmesh");
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->type = ITEM_TYPE_ARMOUR;
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->equipScale = 0.1f;
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->fuelValue = 25;
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->isStackable = false;
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->isUnique = true;
	itemTemplates[ITEM_TIRE_ARMOUR_HEAD]->weight = 2.0f;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_HEAD])->slot = ARMOR_SLOT_HEAD;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_HEAD])->maxDurability = 100;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_HEAD])->damageMitigationMod = 1;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_HEAD])->fireMitigationMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_HEAD])->strengthMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_HEAD])->speedMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_HEAD])->perceptionMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_HEAD])->staminaMod = 0;


	// Tire Breast Plate:
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST] = new ArmourItemTemplate();
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST]->id = ITEM_TIRE_ARMOUR_CHEST;
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST]->name = "Tire Breast Plate";
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST]->invTex = IRR->driver->getTexture("tex/TireArmourChest.bmp");
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST]->mesh = IRR->sceneMgr->getMesh("mdl/Tire.irrmesh");
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST]->type = ITEM_TYPE_ARMOUR;
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST]->equipScale = 0.1f;
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST]->fuelValue = 25;
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST]->isStackable = false;
	itemTemplates[ITEM_TIRE_ARMOUR_CHEST]->isUnique = true;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_CHEST])->slot = ARMOR_SLOT_CHEST;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_CHEST])->maxDurability = 100;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_CHEST])->damageMitigationMod = 1;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_CHEST])->fireMitigationMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_CHEST])->strengthMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_CHEST])->speedMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_CHEST])->perceptionMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_CHEST])->staminaMod = 0;

	// Tire Pants:
	itemTemplates[ITEM_TIRE_ARMOUR_LEG] = new ArmourItemTemplate();
	itemTemplates[ITEM_TIRE_ARMOUR_LEG]->id = ITEM_TIRE_ARMOUR_LEG;
	itemTemplates[ITEM_TIRE_ARMOUR_LEG]->name = "Tire Pants";
	itemTemplates[ITEM_TIRE_ARMOUR_LEG]->invTex = IRR->driver->getTexture("tex/TireArmourLeg.bmp");
	itemTemplates[ITEM_TIRE_ARMOUR_LEG]->mesh = IRR->sceneMgr->getMesh("mdl/Tire.irrmesh");
	itemTemplates[ITEM_TIRE_ARMOUR_LEG]->type = ITEM_TYPE_ARMOUR;
	itemTemplates[ITEM_TIRE_ARMOUR_LEG]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_TIRE_ARMOUR_LEG]->equipScale = 0.1f;
	itemTemplates[ITEM_TIRE_ARMOUR_LEG]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_TIRE_ARMOUR_LEG]->fuelValue = 25;
	itemTemplates[ITEM_TIRE_ARMOUR_LEG]->isStackable = false;
	itemTemplates[ITEM_TIRE_ARMOUR_LEG]->isUnique = true;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_LEG])->slot = ARMOR_SLOT_LEGS;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_LEG])->maxDurability = 100;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_LEG])->damageMitigationMod = 1;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_LEG])->fireMitigationMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_LEG])->strengthMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_LEG])->speedMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_LEG])->perceptionMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_LEG])->staminaMod = 0;

	// Tire Boots:
	itemTemplates[ITEM_TIRE_ARMOUR_FEET] = new ArmourItemTemplate();
	itemTemplates[ITEM_TIRE_ARMOUR_FEET]->id = ITEM_TIRE_ARMOUR_FEET;
	itemTemplates[ITEM_TIRE_ARMOUR_FEET]->name = "Tire Boots";
	itemTemplates[ITEM_TIRE_ARMOUR_FEET]->invTex = IRR->driver->getTexture("tex/TireArmourFeet.bmp");
	itemTemplates[ITEM_TIRE_ARMOUR_FEET]->mesh = IRR->sceneMgr->getMesh("mdl/Tire.irrmesh");
	itemTemplates[ITEM_TIRE_ARMOUR_FEET]->type = ITEM_TYPE_ARMOUR;
	itemTemplates[ITEM_TIRE_ARMOUR_FEET]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_TIRE_ARMOUR_FEET]->equipScale = 0.1f;
	itemTemplates[ITEM_TIRE_ARMOUR_FEET]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_TIRE_ARMOUR_FEET]->fuelValue = 25;
	itemTemplates[ITEM_TIRE_ARMOUR_FEET]->isStackable = false;
	itemTemplates[ITEM_TIRE_ARMOUR_FEET]->isUnique = true;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_FEET])->slot = ARMOR_SLOT_FEET;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_FEET])->maxDurability = 100;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_FEET])->damageMitigationMod = 1;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_FEET])->fireMitigationMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_FEET])->strengthMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_FEET])->speedMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_FEET])->perceptionMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_TIRE_ARMOUR_FEET])->staminaMod = 0;

	// Metal Helm:
	itemTemplates[ITEM_METAL_ARMOUR_HEAD] = new ArmourItemTemplate();
	itemTemplates[ITEM_METAL_ARMOUR_HEAD]->id = ITEM_METAL_ARMOUR_HEAD;
	itemTemplates[ITEM_METAL_ARMOUR_HEAD]->name = "Metal Helm";
	itemTemplates[ITEM_METAL_ARMOUR_HEAD]->invTex = IRR->driver->getTexture("tex/MetalArmourHead.bmp");
	itemTemplates[ITEM_METAL_ARMOUR_HEAD]->mesh = IRR->sceneMgr->getMesh("mdl/Tire.irrmesh");
	itemTemplates[ITEM_METAL_ARMOUR_HEAD]->type = ITEM_TYPE_ARMOUR;
	itemTemplates[ITEM_METAL_ARMOUR_HEAD]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_METAL_ARMOUR_HEAD]->equipScale = 0.1f;
	itemTemplates[ITEM_METAL_ARMOUR_HEAD]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_METAL_ARMOUR_HEAD]->fuelValue = 25;
	itemTemplates[ITEM_METAL_ARMOUR_HEAD]->isStackable = false;
	itemTemplates[ITEM_METAL_ARMOUR_HEAD]->isUnique = true;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_HEAD])->slot = ARMOR_SLOT_HEAD;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_HEAD])->maxDurability = 100;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_HEAD])->damageMitigationMod = 1;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_HEAD])->fireMitigationMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_HEAD])->strengthMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_HEAD])->speedMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_HEAD])->perceptionMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_HEAD])->staminaMod = 0;

	// Metal Breast Plate:
	itemTemplates[ITEM_METAL_ARMOUR_CHEST] = new ArmourItemTemplate();
	itemTemplates[ITEM_METAL_ARMOUR_CHEST]->id = ITEM_METAL_ARMOUR_CHEST;
	itemTemplates[ITEM_METAL_ARMOUR_CHEST]->name = "Metal Breast Plate";
	itemTemplates[ITEM_METAL_ARMOUR_CHEST]->invTex = IRR->driver->getTexture("tex/MetalArmourChest.bmp");
	itemTemplates[ITEM_METAL_ARMOUR_CHEST]->mesh = IRR->sceneMgr->getMesh("mdl/Tire.irrmesh");
	itemTemplates[ITEM_METAL_ARMOUR_CHEST]->type = ITEM_TYPE_ARMOUR;
	itemTemplates[ITEM_METAL_ARMOUR_CHEST]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_METAL_ARMOUR_CHEST]->equipScale = 0.1f;
	itemTemplates[ITEM_METAL_ARMOUR_CHEST]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_METAL_ARMOUR_CHEST]->fuelValue = 25;
	itemTemplates[ITEM_METAL_ARMOUR_CHEST]->isStackable = false;
	itemTemplates[ITEM_METAL_ARMOUR_CHEST]->isUnique = true;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_CHEST])->slot = ARMOR_SLOT_CHEST;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_CHEST])->maxDurability = 100;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_CHEST])->damageMitigationMod = 1;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_CHEST])->fireMitigationMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_CHEST])->strengthMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_CHEST])->speedMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_CHEST])->perceptionMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_CHEST])->staminaMod = 0;

	// Metal Pants:
	itemTemplates[ITEM_METAL_ARMOUR_LEG] = new ArmourItemTemplate();
	itemTemplates[ITEM_METAL_ARMOUR_LEG]->id = ITEM_METAL_ARMOUR_LEG;
	itemTemplates[ITEM_METAL_ARMOUR_LEG]->name = "Metal Pants";
	itemTemplates[ITEM_METAL_ARMOUR_LEG]->invTex = IRR->driver->getTexture("tex/MetalArmourLeg.bmp");
	itemTemplates[ITEM_METAL_ARMOUR_LEG]->mesh = IRR->sceneMgr->getMesh("mdl/Tire.irrmesh");
	itemTemplates[ITEM_METAL_ARMOUR_LEG]->type = ITEM_TYPE_ARMOUR;
	itemTemplates[ITEM_METAL_ARMOUR_LEG]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_METAL_ARMOUR_LEG]->equipScale = 0.1f;
	itemTemplates[ITEM_METAL_ARMOUR_LEG]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_METAL_ARMOUR_LEG]->fuelValue = 25;
	itemTemplates[ITEM_METAL_ARMOUR_LEG]->isStackable = false;
	itemTemplates[ITEM_METAL_ARMOUR_LEG]->isUnique = true;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_LEG])->slot = ARMOR_SLOT_LEGS;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_LEG])->maxDurability = 100;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_LEG])->damageMitigationMod = 1;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_LEG])->fireMitigationMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_LEG])->strengthMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_LEG])->speedMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_LEG])->perceptionMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_LEG])->staminaMod = 0;

	// Metal Boots:
	itemTemplates[ITEM_METAL_ARMOUR_FEET] = new ArmourItemTemplate();
	itemTemplates[ITEM_METAL_ARMOUR_FEET]->id = ITEM_TIRE_ARMOUR_FEET;
	itemTemplates[ITEM_METAL_ARMOUR_FEET]->name = "Metal Boots";
	itemTemplates[ITEM_METAL_ARMOUR_FEET]->invTex = IRR->driver->getTexture("tex/MetalArmourFeet.bmp");
	itemTemplates[ITEM_METAL_ARMOUR_FEET]->mesh = IRR->sceneMgr->getMesh("mdl/Tire.irrmesh");
	itemTemplates[ITEM_METAL_ARMOUR_FEET]->type = ITEM_TYPE_ARMOUR;
	itemTemplates[ITEM_METAL_ARMOUR_FEET]->equipPos = vector3df(0.2f,-0.1f,0.3f);
	itemTemplates[ITEM_METAL_ARMOUR_FEET]->equipScale = 0.1f;
	itemTemplates[ITEM_METAL_ARMOUR_FEET]->equipRot = vector3df(90,90,0);
	itemTemplates[ITEM_METAL_ARMOUR_FEET]->fuelValue = 25;
	itemTemplates[ITEM_METAL_ARMOUR_FEET]->isStackable = false ;
	itemTemplates[ITEM_METAL_ARMOUR_FEET]->isUnique = true;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_FEET])->slot = ARMOR_SLOT_FEET;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_FEET])->maxDurability = 100;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_FEET])->damageMitigationMod = 1;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_FEET])->fireMitigationMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_FEET])->strengthMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_FEET])->speedMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_FEET])->perceptionMod = 0;
	((ArmourItemTemplate*)itemTemplates[ITEM_METAL_ARMOUR_FEET])->staminaMod = 0;
}

BaseItem* ItemManager :: CreateItem(int itemTemplateID)
{
	if(itemTemplates[itemTemplateID]->isUnique)
	{
		BaseItem* newItem = 0;

		if(itemTemplates[itemTemplateID]->type == ITEM_TYPE_ARMOUR)
			newItem = new ArmourItem((ArmourItemTemplate*)itemTemplates[itemTemplateID]);
		else if(itemTemplates[itemTemplateID]->type == ITEM_TYPE_PROJECTILE_WEAPON)
			newItem = new ProjectileWeaponItem((ProjectileWeaponItemTemplate*)itemTemplates[itemTemplateID]);
		else if(itemTemplates[itemTemplateID]->type == ITEM_TYPE_TOOL)
			newItem = new ToolItem((ToolItemTemplate*)itemTemplates[itemTemplateID]);
        else if(itemTemplates[itemTemplateID]->type == ITEM_TYPE_MELEE_WEAPON)
			newItem = new MeleeWeaponItem((MeleeWeaponItemTemplate*)itemTemplates[itemTemplateID]);
        else if(itemTemplates[itemTemplateID]->type == ITEM_TYPE_THROWN_WEAPON)
			newItem = new ThrownWeaponItem((ThrownWeaponItemTemplate*)itemTemplates[itemTemplateID]);

		Register(newItem);
		return newItem;
	}
	else
	{
		if(ItemExists(itemTemplateID))
		{
			// If the item is Non-Unique and it has already been created then simply return a pointer to it.
			return GetItem(itemTemplateID);
		}
		else
		{
			// Otherwise if the item is Non-Unique and it has not been created then it must be created with its associated templateID
			if(itemTemplates[itemTemplateID]->type == ITEM_TYPE_BASE)
				Register(new BaseItem(itemTemplateID, itemTemplates[itemTemplateID]));
			else if(itemTemplates[itemTemplateID]->type == ITEM_TYPE_BLOCK)
				Register(new BlockItem(itemTemplateID, (BlockItemTemplate*)itemTemplates[itemTemplateID]));
			else if(itemTemplates[itemTemplateID]->type == ITEM_TYPE_CONSUMABLE)
				Register(new ConsumableItem(itemTemplateID, (ConsumableItemTemplate*)itemTemplates[itemTemplateID]));
			else if(itemTemplates[itemTemplateID]->type == ITEM_TYPE_AMMO)
				Register(new AmmoItem(itemTemplateID, (AmmoItemTemplate*)itemTemplates[itemTemplateID]));

			return GetItem(itemTemplateID);
		}
	}
}

BaseItem* ItemManager :: GetItem(int id) const
{
	//find the entity
	std::map<int, BaseItem*> :: const_iterator entity = items.find(id);

	assert(entity != items.end());

	return entity->second;
}

void ItemManager :: Remove(BaseItem* i)
{
	items.erase( items.find(i->GetItemID()) );
}

bool ItemManager :: ItemExists(int itemID)
{
	return items.count(itemID) == 1;
}

bool ItemManager :: ItemTemplateExists(int itemTemplateID)
{
	return itemTemplates.count(itemTemplateID) == 1;
}

void  ItemManager :: Register(BaseItem* i)
{
	items.insert(std::make_pair(i->GetItemID(), i));
}

