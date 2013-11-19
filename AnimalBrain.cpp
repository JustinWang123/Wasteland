#include "AnimalBrain.h"
#include <iostream>
#include "CharacterInstance.h"
#include "CommandWander.h"

int AnimalBrain :: THINK_TIME_DELAY = 1000.0f;

void AnimalBrain :: Activate()
{
	// The brain is always active:
	state = active;
}

CommandState AnimalBrain :: Process()
{
	ActivateIfInactive();

	CommandState subCommandState = ProcessSubCommands();

	// if the animal is not currently running a subcommand then determine what the next command is:
	if((subCommandState == completed || subCommandState == failed))
	{
		AddSubCommand(new CommandWander(owner));
	}

	return state;
}

bool AnimalBrain :: HandleMessage(const Telegram& msg)
{
	if(!subCommandList.empty() && subCommandList.front()->HandleMessage(msg))
	{
		return true;
	}

	return false;
}
