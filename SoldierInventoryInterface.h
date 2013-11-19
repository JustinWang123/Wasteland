#ifndef SOLDIER_INVENTORY_INTERFACE_H
#define SOLDIER_INVENTORY_INTERFACE_H

#include "IrrWrapper.h"
#include "Button.h"
#include "ItemSlot.h"
class CharacterInstanceSoldier;
class ItemList;

const int INVENTORY_VISIBLE_SIZE = 9;

class SoldierInventoryInterface
{
public:
	SoldierInventoryInterface();
	~SoldierInventoryInterface(){}

	void SetInterfacePosition(vector2di setInventoryListPos, vector2di setHotbarPos);
	void SetSoldier(CharacterInstanceSoldier* setSoldier) { pSoldier = setSoldier;}

	// Update:
	void UpdateInventoryManagement();
	void UpdateHotbarSelect();

	// Draw:
	void DrawHotbarSlots();
	void DrawInventoryList();
	void DrawMouseSlot();


private:
	bool IsMouseOnHotbar();
	bool IsMouseOnHUD();
	void DrawHUDBackground(vector2di pos, vector2di numOfTiles);

	// These vectors determine where the interface will be drawn onscreen:
	vector2di inventoryListPos;
	vector2di hotbarPos;

	// Pointers to soldier:
	CharacterInstanceSoldier* pSoldier;

	// The interface only displays [x] slots from the soldiers inventory list:
	// This index determines the topmost slot displayed:
	int topInventorySlotIndex;

	// This ItemSlot is used to hold whatever item the player his picked up with the mouse:
	ItemSlot mouseSlot;

	// Buttons:
	Button invListUpButton;
	Button invListDownButton;
	Button invListButtons[INVENTORY_VISIBLE_SIZE];
	Button hotBarButtons[5];
	Button armorSlotButtons[4];

	// Textures:
	ITexture* hotbarSelectTexture;
	ITexture* hudTopLeft;
	ITexture* hudTop;
	ITexture* hudTopRight;
	ITexture* hudRight;
	ITexture* hudBottemRight;
	ITexture* hudBottem;
	ITexture* hudBottemLeft;
	ITexture* hudLeft;
	ITexture* hudMiddle;
};

#endif
