#include "CommandIdle.h"
#include "CharacterInstance.h"

CommandIdle  :: CommandIdle(CharacterInstance* owner, float idleLength)
	: CharacterCommand(owner, COMMAND_IDLE)
{
	state = inactive;

	idleStopTime = IRR->GetTime() + idleLength;
}

void CommandIdle  :: Activate()
{
	state = active;
}

CommandState  CommandIdle  :: Process()
{
	// If this is the first time processing, then activate:
	ActivateIfInactive();

	if(IRR->GetTime() > idleStopTime)
		state = completed;

	return state;
}

void CommandIdle  :: Terminate()
{

}
