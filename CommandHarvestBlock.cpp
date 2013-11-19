#include "CommandHarvestBlock.h"
#include "CharacterInstance.h"
#include "Telegram.h"
#include "CommandPathToPosition.h"
#include "MapManager.h"
#include <iostream>


CommandHarvestBlock  :: CommandHarvestBlock(vector3di setTargetBlock)
	:   CompoundCharacterCommand(0, COMMAND_HARVEST_BLOCK),
        targetBlock(setTargetBlock),
        targetBlockPos(setTargetBlock.X + 0.5f, setTargetBlock.Y + 0.5f, setTargetBlock.Z + 0.5f)

{
	debugObject = IRR->sceneMgr->addCubeSceneNode(1.05f,0,0, targetBlockPos);
	debugObject->setVisible(true);
	debugObject->setMaterialFlag(EMF_LIGHTING, false);
	debugObject->setMaterialFlag(EMF_WIREFRAME, true);
}

void CommandHarvestBlock  :: Activate()
{
    if(MapMgr->IsBlockOpen(targetBlock))
    {
		std::cout << "CommandHarvestBlock::Activate() - Fail\n";
		state = failed;
    }
    else
    {
		std::cout << "CommandHarvestBlock::Activate() - Success\n";
		state = active;
    }
}

CommandState  CommandHarvestBlock  :: Process()
{
	ActivateIfInactive();

	ProcessSubCommands();

	if(owner->GetFeetPosition().getDistanceFrom(targetBlockPos) > 1.0f
	&& !IsCurrentCommand(COMMAND_PATH_TO_POSITION))
	{
		AddSubCommand(new CommandPathToPosition(owner, targetBlockPos));
	}
	else
	{
		state = completed;
	}

	return state;
}


void CommandHarvestBlock :: Terminate()
{
	RemoveAllSubCommands();
}

bool CommandHarvestBlock :: HandleMessage(const Telegram& msg)
{
	return false;
}

bool CommandHarvestBlock :: IsEqual(CharacterCommand* command)
{
	if(command->GetType() == COMMAND_HARVEST_BLOCK
	&& ((CommandHarvestBlock*)command)->GetTargetBlock() == targetBlock)
	{
		return true;
	}
	else
	{
		return false;
	}
}


