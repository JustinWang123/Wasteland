#include "CommandAttackCharacter.h"
#include "CharacterInstance.h"
#include "Telegram.h"
#include "InstanceManager.h"
#include "CommandPathToCharacter.h"
#include "CharacterInstanceSoldier.h"
#include <iostream>

CommandAttackCharacter  :: CommandAttackCharacter(CharacterInstance* owner, CharacterInstance* setTarget)
	:   CompoundCharacterCommand(owner, COMMAND_ATTACK_CHARACTER),
        target(setTarget)
        {}

void CommandAttackCharacter  :: Activate()
{
    CharacterInstanceSoldier* soldierOwner = (CharacterInstanceSoldier*)owner;

    // Command fails if there is no valid target or the soldier does not have a weapon selected:
	if(target == 0
	||  !( soldierOwner->GetSelectedItemType() == ITEM_TYPE_PROJECTILE_WEAPON
        || soldierOwner->GetSelectedItemType() == ITEM_TYPE_MELEE_WEAPON
        || soldierOwner->GetSelectedItemType() == ITEM_TYPE_THROWN_WEAPON
        || soldierOwner->GetSelectedItemType() == ITEM_TYPE_NONE))
	{
		std::cout << "CommandAttackCharacter::Activate() - Fail\n";

		state = failed;
	}
	else
	{
		std::cout << "CommandAttackCharacter::Activate() - Success\n";

		state = active;

		((CharacterInstanceSoldier*)owner)->SetAttackTarget(target);
	}
}

CommandState  CommandAttackCharacter  :: Process()
{
    CharacterInstanceSoldier* soldierOwner = (CharacterInstanceSoldier*)owner;

	ActivateIfInactive();

	ProcessSubCommands();

	if(soldierOwner->GetSelectedItemType() == ITEM_TYPE_PROJECTILE_WEAPON
	&& soldierOwner->IsClipEmpty())
	{
		soldierOwner->ReloadProjectileWeapon();
	}

	if(soldierOwner->GetAccuracy(target) > 0)
	{
		AttackTarget();
	}
	else if(!IsCurrentCommand(COMMAND_PATH_TO_CHARACTER))
	{
		PursueTarget();
	}

	if(target->IsDead())
	{
        std::cout << "CommandAttackCharacter::Target->IsDead()\n";
		state = completed;
	}

	return state;
}



void CommandAttackCharacter :: PursueTarget()
{
    std::cout << "CommandAttackCharacter::PursueTarget()\n";
	RemoveAllSubCommands();
	AddSubCommand(new CommandPathToCharacter(owner, target, 1.0f));
}

void CommandAttackCharacter :: FaceTarget()
{
	owner->SetHeading(target->GetFeetPosition() - owner->GetFeetPosition());
	owner->SetLookAtHeading(target->GetBodyPosition() - owner->GetBodyPosition());
}

void CommandAttackCharacter :: AttackTarget()
{
	FaceTarget();
	RemoveAllSubCommands();
	((CharacterInstanceSoldier*)owner)->UseCurrentlySelectedItem();
}

void CommandAttackCharacter :: Terminate()
{
	RemoveAllSubCommands();
}

bool CommandAttackCharacter :: HandleMessage(const Telegram& msg)
{
	return false;
}
