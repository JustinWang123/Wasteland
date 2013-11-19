#include "CommandWander.h"
#include "CharacterInstance.h"
#include "CommandMoveToRandomPosition.h"
#include "CommandIdle.h"

CommandWander  :: CommandWander(CharacterInstance* owner)
	: CompoundCharacterCommand(owner, COMMAND_WANDER)
{
	state = inactive;
}

void CommandWander  :: Activate()
{
	state = active;
}

CommandState  CommandWander  :: Process()
{
	// If this is the first time processing, then activate:
	ActivateIfInactive();

	CommandState subCommandState = ProcessSubCommands();

	// if the animal is not currently running a subcommand then determine what the next command is:
	if((subCommandState == completed || subCommandState == failed))
	{
		int choice = rand() % 2;

		if(choice == 0)
		{
			AddSubCommand(new CommandMoveToRandomPosition(owner, 10));
		}
		if(choice == 1)
		{
			AddSubCommand(new CommandIdle(owner, 3000));
		}
	}

	return state;
}

void CommandWander  :: Terminate()
{
	RemoveAllSubCommands();
}
