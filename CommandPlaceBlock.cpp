#include "GameManager.h"
#include "NonPlayerCharacterInstance.h"
#include <iostream>

CommandPlaceBlock  :: CommandPlaceBlock(NonPlayerCharacterInstance* owner, vector3di setBlock, int setHotBarSlot)
	: CharacterCommand(owner, DESTROY_BLOCK)
{
	hotBarSlot = setHotBarSlot;
	block = setBlock;
	debugObject = IRR->sceneMgr->addCubeSceneNode(1.02f, 0, -1, vector3df(block.X + 0.5f, block.Y + 0.5f, block.Z + 0.5f));
	debugObject->setMaterialFlag(EMF_WIREFRAME, true);
	state = inactive;
}

void CommandPlaceBlock  :: Activate()
{
	state = active;
}

CommandState  CommandPlaceBlock  :: Process()
{
	// If this is the first time processing, then activate:
	ActivateIfInactive();

	if( (owner->GetSceneNode()->getPosition()).getDistanceFrom(vector3df(block.X + 0.5f, block.Y, block.Z + 0.5f)) > 2.0f)
	{
		std::cout << "CommandPlaceBlock - failed - to far from block" << std::endl;
		state = failed;
	}
	else if( MapMgr->IsBlockSolid(vector3df(block.X, block.Y, block.Z)) )
	{

		std::cout << "CommandPlaceBlock - failed - block is not open" << std::endl;
		state = failed;
	}
	/*
	else if(owner->GetInventorySlot(hotBarSlot, HOT_BAR_Y)->IsEmpty()
	|| owner->GetInventorySlot(hotBarSlot, HOT_BAR_Y)->GetType() != ITEM_TYPE_BLOCK)
	{
		std::cout << "CommandPlaceBlock - failed - inventory issue" << std::endl;
		state = failed;
	}
	*/
	/*
	else if(owner->GetActionDelay() <= 0)
	{
		std::cout << "CommandPlaceBlock - suceeded - placing block" << std::endl;
		//owner->PlaceBlock(block, hotBarSlot);
		state = completed;
	}
	*/
	return state;

}

void CommandPlaceBlock  :: Terminate()
{
}
