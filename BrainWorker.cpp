#include "BrainWorker.h"

void BrainWorker :: Activate()
{
	state = active;
}

CommandState BrainWorker :: Process()
{
	ActivateIfInactive();

	CommandState subCommandState = ProcessSubCommands();

	if (subCommandState == completed || subCommandState == failed)
	{
		state = inactive;
	}

  return state;
}

bool BrainWorker :: HandleMessage(const Telegram& msg)
{
	if(!subCommandList.empty() && subCommandList.front()->HandleMessage(msg))
	{
		return true;
	}

	return false;
}

