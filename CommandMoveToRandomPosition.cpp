#include "CommandMoveToRandomPosition.h"
#include "CommandMoveToPosition.h"
#include "PathPlanner.h"
#include "CharacterInstance.h"

CommandMoveToRandomPosition  :: CommandMoveToRandomPosition(CharacterInstance* owner, float setMaxDistance)
	: CompoundCharacterCommand(owner, COMMAND_PATH_TO_CHARACTER)
{
	state = inactive;
	maxDistance = setMaxDistance;
}

void CommandMoveToRandomPosition  :: Activate()
{
	state = active;

	std::list<vector3df> path;

	// note the vector added is to gurenteed that the starting voxel is the one the player is standing within
	PPlanner->GetPathToRandomPos(owner->GetFeetPosition(), maxDistance, &path);

	if(!path.empty())
	{

		while(!path.empty())
		{
			AddSubCommand(new CommandMoveToPosition(owner, path.back()));
			path.pop_back();
		}


		state = active;
	}
	else
	{
		state = failed;
	}
}

CommandState  CommandMoveToRandomPosition  :: Process()
{
	// If this is the first time processing, then activate:
	ActivateIfInactive();

	state = ProcessSubCommands();

	return state;
}

void CommandMoveToRandomPosition :: Terminate()
{
	RemoveAllSubCommands();
}
