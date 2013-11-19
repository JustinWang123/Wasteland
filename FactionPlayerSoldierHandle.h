#ifndef FACTION_PLAYER_SOLDIER_HANDLE_H
#define FACTION_PLAYER_SOLDIER_HANDLE_H

#include "SoldierInventoryInterface.h"
class CharacterInstanceSoldier;

struct FactionPlayerSoldierHandle
{
	FactionPlayerSoldierHandle(CharacterInstanceSoldier* setSoldier, vector2di invPos, vector2di hotbarPos)
		: soldier(setSoldier)
	{
		inventoryInterface = new SoldierInventoryInterface(setSoldier, invPos, hotbarPos);
	}

	~FactionPlayerSoldierHandle()
	{
		delete inventoryInterface;
	}

	CharacterInstanceSoldier* soldier;
	SoldierInventoryInterface* inventoryInterface;
	bool isSelected;
};

#endif
