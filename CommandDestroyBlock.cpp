#include "CommandDestroyBlock.h"
#include "GameManager.h"
#include "CharacterInstance.h"
#include "CharacterInstance.h"
#include <iostream>

CommandDestroyBlock  :: CommandDestroyBlock(CharacterInstance* owner, vector3di setBlock, int setHotBarSlot)
	: CharacterCommand(owner, DESTROY_BLOCK)
{
	hotBarSlot = setHotBarSlot;
	block = setBlock;
	debugObject = IRR->sceneMgr->addCubeSceneNode(1.02f, 0, -1, vector3df(block.X + 0.5f, block.Y + 0.5f, block.Z + 0.5f));
	debugObject->setMaterialFlag(EMF_WIREFRAME, true);
	state = inactive;
}

void CommandDestroyBlock  :: Activate()
{
	state = active;
}

CommandState  CommandDestroyBlock  :: Process()
{
	// If this is the first time processing, then activate:
	ActivateIfInactive();

	if( (owner->GetSceneNode()->getPosition()).getDistanceFrom(vector3df(block.X + 0.5f, block.Y, block.Z + 0.5f)) > 2.0f)
	{
		std::cout << "CommandDestroyBlock - Failed - to far from block" << std::endl;
		state = failed;
	}
	else if( !MapMgr->IsBlockSolid(vector3df(block.X, block.Y, block.Z)) )
	{

		std::cout << "CommandDestroyBlock - Suceeded - block is now open" << std::endl;
		state = completed;
	}
	/*
	else if(owner->GetActionDelay() <= 0)
	{
		//owner->DamageBlock(block, hotBarSlot);
	}
	*/

	return state;

}

void CommandDestroyBlock  :: Terminate()
{
}
