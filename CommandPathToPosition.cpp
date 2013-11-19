#include "CommandPathToPosition.h"
#include "CharacterInstance.h"
#include "CommandMoveToPosition.h"
#include "PathPlanner.h"
#include <iostream>

CommandPathToPosition  :: CommandPathToPosition(CharacterInstance* owner, vector3df setPosition)
	: CompoundCharacterCommand(owner, COMMAND_MOVE_TO_POSITION)
{
	state = inactive;
	position = setPosition;
}

void CommandPathToPosition  :: Activate()
{
	std::list<vector3df> path;

	// note the vector added is to gurenteed that the starting voxel is the one the player is standing within
	PPlanner->GetPathToPos(owner->GetFeetPosition(), position, &path);

    int pathSize = path.size();
	for(int i = 0; i < pathSize; i++)
	{
		AddSubCommand(new CommandMoveToPosition(owner, path.back()));
		path.pop_back();
	}

	state = active;

}

CommandState  CommandPathToPosition  :: Process()
{
	// If this is the first time processing, then activate:
	ActivateIfInactive();

	state = ProcessSubCommands();

	return state;
}

void CommandPathToPosition  :: Terminate()
{
	RemoveAllSubCommands();
}
