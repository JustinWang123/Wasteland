#include "CommandMoveToPosition.h"
#include "CharacterInstance.h"

CommandMoveToPosition  :: CommandMoveToPosition(CharacterInstance* owner, vector3df setPosition)
	: CharacterCommand(owner, COMMAND_PATH_TO_POSITION)
{
	state = inactive;

	position = setPosition;

	debugObject = IRR->sceneMgr->addCubeSceneNode(0.1,0,0, setPosition);
	debugObject->setVisible(true);
	debugObject->setMaterialFlag(EMF_LIGHTING, false);
}

void CommandMoveToPosition  :: Activate()
{
	state = active;

	owner->SetIsMoving(true);
	owner->SetDestination(position);
}

CommandState  CommandMoveToPosition  :: Process()
{
	// If this is the first time processing, then activate:
	ActivateIfInactive();

	// If arrived at destination then complete command:
	if( owner->GetFeetPosition().getDistanceFrom(position) <= 0.2f)
	{
		state = completed;
	}

	return state;
}

void CommandMoveToPosition  :: Terminate()
{
	owner->SetIsMoving(false);
}
