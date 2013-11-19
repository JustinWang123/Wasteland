#include "ZombieBrain.h"
#include "CharacterInstance.h"
#include "InstanceManager.h"
#include "CommandAttackCharacter.h"
#include <iostream>
#include "CharacterInstance.h"
#include "CommandWander.h"
#include <stdlib.h>

int ZombieBrain :: THINK_TIME_DELAY = 1000;

void ZombieBrain :: Activate()
{
	state = active;

	thinkTime = IRR->GetTime() + THINK_TIME_DELAY;
}

CommandState ZombieBrain :: Process()
{
	ActivateIfInactive();

	ProcessSubCommands();

	// if the zombie is not currently running a subcommand then determine what the next command is:
	if(IRR->GetTime() > thinkTime)
	{
		if(!IsCurrentCommand(COMMAND_ATTACK_CHARACTER))
		{
			CharacterInstance* target = GetTarget();

			if(target != 0)
			{
				std::cout << "Target aquired!" << std::endl;
				RemoveAllSubCommands();
				AddSubCommand(new CommandAttackCharacter(owner, target));
			}
		}

		if(!IsCurrentCommand(COMMAND_WANDER) && !IsCurrentCommand(COMMAND_ATTACK_CHARACTER))
		{
			RemoveAllSubCommands();
			AddSubCommand(new CommandWander(owner));
		}

		thinkTime = IRR->GetTime() + THINK_TIME_DELAY;
	}

	return state;
}

CharacterInstance*	ZombieBrain :: GetTarget()
{

	static float AGRO_RADIUS = 6.0f;

	std::list<int> charactersInRange;

	InstanceMgr->GetCharactersInRadius(charactersInRange, owner->GetFeetPosition(), AGRO_RADIUS);

	int chunkID;
	vector3df impactPos;

	for(std::list<int>::iterator it = charactersInRange.begin(); it != charactersInRange.end(); it++)
	{
		CharacterInstance* testAgroChar = InstanceMgr->GetCharacterFromID(*it);

		float distance = testAgroChar->GetHeadPosition().getDistanceFrom(owner->GetHeadPosition());

		if(InstanceMgr->IsHostile(owner->GetFactionID(), testAgroChar->GetFactionID())
		&& !InstanceMgr->GetChunkPosHitByRay(chunkID, impactPos, owner->GetHeadPosition(), testAgroChar->GetHeadPosition() - owner->GetHeadPosition(), distance))
		{
			return testAgroChar;
		}
	}

	return 0;
}

bool ZombieBrain :: HandleMessage(const Telegram& msg)
{
	if(!subCommandList.empty() && subCommandList.front()->HandleMessage(msg))
	{
		return true;
	}

	return false;
}
