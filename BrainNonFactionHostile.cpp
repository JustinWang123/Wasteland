#include "BrainNonFactionHostile.h"
#include "CharacterInstanceSoldier.h"
#include "InstanceManager.h"
#include "CommandAttackCharacter.h"
#include "CommandWander.h"
#include <iostream>
#include <stdlib.h>

int BrainNonFactionHostile :: THINK_TIME_DELAY = 1000;

void BrainNonFactionHostile :: Activate()
{
	state = active;

	thinkTime = IRR->GetTime() + THINK_TIME_DELAY;
}

CommandState BrainNonFactionHostile :: Process()
{
	ActivateIfInactive();

	ProcessSubCommands();

	// if the character is not currently running a subcommand then determine what the next command is:
	if(IRR->GetTime() > thinkTime)
	{
		if(!IsCurrentCommand(COMMAND_ATTACK_CHARACTER))
		{
			CharacterInstance* target = GetAgroTarget();

			if(target != 0)
			{
				std::cout << "Target aquired!" << std::endl;
				RemoveAllSubCommands();

				// Equip best possible weapon:
				CharacterInstanceSoldier* soldierOwner = (CharacterInstanceSoldier*)owner;
				if(!( soldierOwner->GetSelectedItemType() == ITEM_TYPE_PROJECTILE_WEAPON
				|| soldierOwner->GetSelectedItemType() == ITEM_TYPE_MELEE_WEAPON))
				{
					if(!soldierOwner->EquipBestProjectileWeapon())
					{
						soldierOwner->EquipBestMeleeWeapon();
					}
				}

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

CharacterInstance*	BrainNonFactionHostile :: GetAgroTarget()
{

	static float AGRO_RADIUS = 20.0f;

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

bool BrainNonFactionHostile :: HandleMessage(const Telegram& msg)
{
	if(!subCommandList.empty() && subCommandList.front()->HandleMessage(msg))
	{
		return true;
	}

	return false;
}
