#include "BrainSoldier.h"
//#include "CharacterInstance.h"
//#include "GameManager.h"


void BrainSoldier :: Activate()
{
	state = active;
}

CommandState BrainSoldier :: Process()
{
	ActivateIfInactive();

	CommandState subCommandState = ProcessSubCommands();

	if (subCommandState == completed || subCommandState == failed)
	{
		state = inactive;
	}

  return state;
}

bool BrainSoldier :: HandleMessage(const Telegram& msg)
{
	if(!subCommandList.empty() && subCommandList.front()->HandleMessage(msg))
	{
		return true;
	}

	return false;
}
