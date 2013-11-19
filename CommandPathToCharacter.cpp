#include "CommandPathToCharacter.h"
#include "CharacterInstance.h"
#include "Telegram.h"
#include "InstanceManager.h"
#include "CommandPathToPosition.h"
#include <iostream>

CommandPathToCharacter  :: CommandPathToCharacter(CharacterInstance* owner, CharacterInstance* setTarget, float setDistance)
	:   CompoundCharacterCommand(owner, COMMAND_PATH_TO_CHARACTER),
        target(setTarget),
        distance(setDistance)
        {}

void CommandPathToCharacter  :: Activate()
{
    std::cout << "CommandPathToCharacter::Activate()\n";

	state = active;

	if(target == 0)
	{
		state = failed;
	}

	if(target->GetFeetPosition().getDistanceFrom(owner->GetFeetPosition()) > distance)
	{
		GetPathToTarget();
	}
	else
	{
		state = completed;
	}
}

CommandState  CommandPathToCharacter  :: Process()
{
	// If this is the first time processing, then activate:
	ActivateIfInactive();

	int subCommandState = ProcessSubCommands();

	// The subCommand (PathToPosition) is failed it means could not find a path
	if(subCommandState == failed || subCommandState == completed)
	{
		GetPathToTarget();
	}

	// The target has moved (1.0f) away from the current destination
	if(target->GetFeetPosition().getDistanceFrom(currentPathDestination) > 1.0f)
	{
		GetPathToTarget();
	}

	// The owner has arrived at the targets position
	if(target->GetFeetPosition().getDistanceFrom(owner->GetFeetPosition()) < distance)
	{
		state = completed;
	}

	return state;
}

void CommandPathToCharacter :: Terminate()
{
    std::cout << "CommandPathToCharacter::Terminate()\n";
	RemoveAllSubCommands();
}

void CommandPathToCharacter  :: GetPathToTarget()
{
	RemoveAllSubCommands();

	currentPathDestination = target->GetFeetPosition();

	AddSubCommand(new CommandPathToPosition(owner, target->GetFeetPosition()));
}

bool CommandPathToCharacter :: HandleMessage(const Telegram& msg)
{
	return false;
}
