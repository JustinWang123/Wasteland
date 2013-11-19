#include "CharacterCommand.h"

CharacterCommand :: CharacterCommand(CharacterInstance* setOwner, CommandType setType)
	:   type(setType),
        state(inactive),
        owner(setOwner),
        debugObject(0)
        {}


CharacterCommand :: ~CharacterCommand()
{
	if(debugObject != 0)
		debugObject->remove();
}

void  CharacterCommand :: ReactivateIfFailed()
{
	if (HasFailed())
	{
		state = inactive;
	}
}

void  CharacterCommand :: ActivateIfInactive()
{
	if (IsInactive())
	{
		Activate();
	}
}
