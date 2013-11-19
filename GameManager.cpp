#include "GameManager.h"
#include "PathPlanner.h"
#include "ItemManager.h"
#include "InstanceManager.h"
#include "FactionManager.h"
#include "Faction.h"
#include "CharacterInstance.h"
#include "CharacterInstanceSoldier.h"
#include "CharacterInstanceWorker.h"
#include "BrainNonFactionHostile.h"
#include "AnimalBrain.h"
#include "BrainSoldier.h"
#include "BrainWorker.h"
#include "CommandPathToPosition.h"

using namespace std;

GameManager :: GameManager()
{

}

GameManager* GameManager :: Instance()
{
	static GameManager instance;
	return &instance;
}

void GameManager :: Init(bool setCreativeModeOn)
{
	isCreativeModeOn = setCreativeModeOn;
    time = 800;
	PPlanner->Init();
    InstanceMgr->Init();
    MapMgr->LoadInitialChunks();
    FactionMgr->Init();

    CharacterInstance* character;
    Faction* faction;

    faction = FactionMgr->CreateFaction(FACTION_TYPE_PLAYER_CONTROL);

	// Create Soldier:
    character = InstanceMgr->CreateCharacterInstanceSoldier(CHARACTER_INSTANCE_TEMPLATE_SOLDIER, vector3df(160,22,65));
    character->SetFactionID(FACTION_ID_PLAYER);
    character->SetBrain(new BrainSoldier(character));
    character->AddToInventory(ItemMgr->CreateItem(ITEM_SHOTGUN), 1);
    character->AddToInventory(ItemMgr->CreateItem(ITEM_SHOTGUN_SHELL), 30);
    character->AddToInventory(ItemMgr->CreateItem(ITEM_MACHINE_GUN), 1);
    character->AddToInventory(ItemMgr->CreateItem(ITEM_762_BULLET), 100);
	character->AddToInventory(ItemMgr->CreateItem(ITEM_SCRAP_PICK), 1);
	character->AddToInventory(ItemMgr->CreateItem(ITEM_MEDICAL_KIT), 1);
	character->AddToInventory(ItemMgr->CreateItem(ITEM_TIRE_ARMOUR_CHEST), 1);
	faction->AddSoldier((CharacterInstanceSoldier*)character);

	// Create Worker:
	character = InstanceMgr->CreateCharacterInstanceWorker(vector3df(160,22,67));
    character->SetFactionID(FACTION_ID_PLAYER);
    character->SetBrain(new BrainWorker(character));
	faction->AddWorker((CharacterInstanceWorker*)character);

	// Make a Hostile:
	character = InstanceMgr->CreateCharacterInstanceSoldier(CHARACTER_INSTANCE_TEMPLATE_ZOMBIE, vector3df(180,22,65));
	character->SetFactionID(FACTION_ID_ZOMBIE);
	character->SetBrain(new BrainNonFactionHostile((CharacterInstanceSoldier*)character));
	character->AddToInventory(ItemMgr->CreateItem(ITEM_PISTOL), 1);
	character->AddToInventory(ItemMgr->CreateItem(ITEM_9_MM_BULLET), 100);
	character->AddToInventory(ItemMgr->CreateItem(ITEM_SCRAP_PICK), 1);

/*
	// Make a Cow:
	character = InstanceMgr->CreateCharacterInstanceSoldier(CHARACTER_INSTANCE_TEMPLATE_COW, vector3df(150,22,65));
	character->SetFactionID(FACTION_ID_COW);
	character->SetBrain(new AnimalBrain(character));

	// Make a Cow:
	character = InstanceMgr->CreateCharacterInstanceSoldier(CHARACTER_INSTANCE_TEMPLATE_COW, vector3df(155,22,65));
	character->SetFactionID(FACTION_ID_COW);
	character->SetBrain(new AnimalBrain(character));

	// Make a Cow:
	character = InstanceMgr->CreateCharacterInstanceSoldier(CHARACTER_INSTANCE_TEMPLATE_COW, vector3df(155,22,60));
	character->SetFactionID(FACTION_ID_COW);
	character->SetBrain(new AnimalBrain(character));


	character = InstanceMgr->CreateCharacterInstanceSoldier(CHARACTER_INSTANCE_TEMPLATE_ZOMBIE, vector3df(180,22,70));
	character->SetFactionID(FACTION_ID_ZOMBIE);
	character->SetBrain(new ZombieBrain(character));

	character = InstanceMgr->CreateCharacterInstanceSoldier(CHARACTER_INSTANCE_TEMPLATE_ZOMBIE, vector3df(180,22,60));
	character->SetFactionID(FACTION_ID_ZOMBIE);
	character->SetBrain(new ZombieBrain(character));
*/

/*
	// Recipe
	for(int i = 0; i < MAX_RECIPES; i++)
		recipeData[i].resultID = -1;

	// Split wood block to planks:
	recipeData[0].itemIDs.push_back(ITEM_WOOD_BLOCK);
	recipeData[0].itemAmounts.push_back(1);
	recipeData[0].toolIDs.push_back(ITEM_SCRAP_SAW);
	recipeData[0].resultID = ITEM_WOOD_PLANK;
	recipeData[0].resultAmount = 4;

	// Split planks to sticks:
	recipeData[1].itemIDs.push_back(ITEM_WOOD_PLANK);
	recipeData[1].itemAmounts.push_back(1);
	recipeData[1].toolIDs.push_back(ITEM_SCRAP_SAW);
	recipeData[1].resultID = ITEM_WOOD_STICK;
	recipeData[1].resultAmount = 4;

	// Split tires into rubber:
	recipeData[2].itemIDs.push_back(ITEM_TIRE);
	recipeData[2].itemAmounts.push_back(1);
	recipeData[2].toolIDs.push_back(ITEM_SCRAP_SAW);
	recipeData[2].resultID = ITEM_RUBBER;
	recipeData[2].resultAmount = 4;

	// Craft Scrap Shovel:
	recipeData[3].itemIDs.push_back(ITEM_WOOD_STICK);
	recipeData[3].itemAmounts.push_back(4);
	recipeData[3].itemIDs.push_back(ITEM_SCRAP_METAL);
	recipeData[3].itemAmounts.push_back(4);
	recipeData[3].resultID = ITEM_SCRAP_SHOVEL;
	recipeData[3].resultAmount = 1;

	// Craft Scrap Pick:
	recipeData[4].itemIDs.push_back(ITEM_WOOD_STICK);
	recipeData[4].itemAmounts.push_back(4);
	recipeData[4].itemIDs.push_back(ITEM_SCRAP_METAL);
	recipeData[4].itemAmounts.push_back(4);
	recipeData[4].resultID = ITEM_SCRAP_PICK;
	recipeData[4].resultAmount = 1;

	// Craft Scrap Metal Cutters:
	recipeData[5].itemIDs.push_back(ITEM_WOOD_STICK);
	recipeData[5].itemAmounts.push_back(4);
	recipeData[5].itemIDs.push_back(ITEM_SCRAP_METAL);
	recipeData[5].itemAmounts.push_back(4);
	recipeData[5].resultID = ITEM_SCRAP_METAL_CUTTERS;
	recipeData[5].resultAmount = 1;

	// Craft Scrap File:
	recipeData[6].itemIDs.push_back(ITEM_WOOD_STICK);
	recipeData[6].itemAmounts.push_back(4);
	recipeData[6].itemIDs.push_back(ITEM_SCRAP_METAL);
	recipeData[6].itemAmounts.push_back(4);
	recipeData[6].resultID = ITEM_SCRAP_FILE;
	recipeData[6].resultAmount = 1;

	// Craft Scrap Saw:
	recipeData[7].itemIDs.push_back(ITEM_WOOD_STICK);
	recipeData[7].itemAmounts.push_back(4);
	recipeData[7].itemIDs.push_back(ITEM_SCRAP_METAL);
	recipeData[7].itemAmounts.push_back(4);
	recipeData[7].resultID = ITEM_SCRAP_SAW;
	recipeData[7].resultAmount = 1;

	// Craft Scrap Hammer:
	recipeData[8].itemIDs.push_back(ITEM_WOOD_STICK);
	recipeData[8].itemAmounts.push_back(4);
	recipeData[8].itemIDs.push_back(ITEM_SCRAP_METAL);
	recipeData[8].itemAmounts.push_back(4);
	recipeData[8].resultID = ITEM_SCRAP_HAMMER;
	recipeData[8].resultAmount = 1;

	// Split tire into rubber:
	recipeData[9].itemIDs.push_back(ITEM_TIRE);
	recipeData[9].itemAmounts.push_back(1);
	recipeData[9].toolIDs.push_back(ITEM_SCRAP_METAL_CUTTERS);
	recipeData[9].resultID = ITEM_RUBBER;
	recipeData[9].resultAmount = 4;

	// Tire Helm:
	recipeData[10].itemIDs.push_back(ITEM_RUBBER);
	recipeData[10].itemAmounts.push_back(4);
	recipeData[10].toolIDs.push_back(ITEM_SCRAP_METAL_CUTTERS);
	recipeData[10].resultID = ITEM_TIRE_ARMOUR_HEAD;
	recipeData[10].resultAmount = 1;

	// Tire Shirt:
	recipeData[11].itemIDs.push_back(ITEM_RUBBER);
	recipeData[11].itemAmounts.push_back(12);
	recipeData[11].toolIDs.push_back(ITEM_SCRAP_METAL_CUTTERS);
	recipeData[11].resultID = ITEM_TIRE_ARMOUR_CHEST;
	recipeData[11].resultAmount = 1;

	// Tire Legs:
	recipeData[12].itemIDs.push_back(ITEM_RUBBER);
	recipeData[12].itemAmounts.push_back(10);
	recipeData[12].toolIDs.push_back(ITEM_SCRAP_METAL_CUTTERS);
	recipeData[12].resultID = ITEM_TIRE_ARMOUR_LEG;
	recipeData[12].resultAmount = 1;

	// Tire Feet:
	recipeData[13].itemIDs.push_back(ITEM_RUBBER);
	recipeData[13].itemAmounts.push_back(4);
	recipeData[13].toolIDs.push_back(ITEM_SCRAP_METAL_CUTTERS);
	recipeData[13].resultID = ITEM_TIRE_ARMOUR_FEET;
	recipeData[13].resultAmount = 1;

	// Tire Shoulders:
	recipeData[14].itemIDs.push_back(ITEM_RUBBER);
	recipeData[14].itemAmounts.push_back(8);
	recipeData[14].toolIDs.push_back(ITEM_SCRAP_METAL_CUTTERS);
	recipeData[14].resultID = ITEM_TIRE_ARMOUR_SHOULDER;
	recipeData[14].resultAmount = 1;

	// Tire Back:
	recipeData[15].itemIDs.push_back(ITEM_RUBBER);
	recipeData[15].itemAmounts.push_back(8);
	recipeData[15].toolIDs.push_back(ITEM_SCRAP_METAL_CUTTERS);
	recipeData[15].resultID = ITEM_TIRE_ARMOUR_BACK;
	recipeData[15].resultAmount = 1;

	// Tire Arms:
	recipeData[16].itemIDs.push_back(ITEM_RUBBER);
	recipeData[16].itemAmounts.push_back(6);
	recipeData[16].toolIDs.push_back(ITEM_SCRAP_METAL_CUTTERS);
	recipeData[16].resultID = ITEM_TIRE_ARMOUR_ARM;
	recipeData[16].resultAmount = 1;

	// Tire Arms:
	recipeData[17].itemIDs.push_back(ITEM_RUBBER);
	recipeData[17].itemAmounts.push_back(4);
	recipeData[17].toolIDs.push_back(ITEM_SCRAP_METAL_CUTTERS);
	recipeData[17].resultID = ITEM_TIRE_ARMOUR_HAND;
	recipeData[17].resultAmount = 1;

	// Furnace:
	recipeData[18].itemIDs.push_back(ITEM_BARREL);
	recipeData[18].itemAmounts.push_back(1);
	recipeData[18].itemIDs.push_back(ITEM_SCRAP_METAL);
	recipeData[18].itemAmounts.push_back(4);
	recipeData[18].toolIDs.push_back(ITEM_SCRAP_METAL_CUTTERS);
	recipeData[18].resultID = ITEM_BARREL_FURNACE;
	recipeData[18].resultAmount = 1;
*/



}

void GameManager :: Destroy()
{
    FactionMgr->Destroy();
	InstanceMgr->Destroy();
	MapMgr->Destroy();
}

void GameManager :: Update()
{
	time++;

	if(time > 24000)
		time = 0;

	MapMgr->Update();
	InstanceMgr->Update();
	ItemMgr->Update();
	FactionMgr->Update();
}
