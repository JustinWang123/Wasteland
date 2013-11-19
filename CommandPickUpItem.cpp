#include "CommandPickUpItem.h"
#include "CharacterInstanceSoldier.h"
#include "ItemInstance.h"
#include "InstanceManager.h"
#include "CommandPathToPosition.h"
#include <iostream>


CommandPickUpItem  :: CommandPickUpItem(CharacterInstance* owner, ItemInstance* setItem)
	:   CompoundCharacterCommand(owner, COMMAND_PICK_UP_ITEM),
        item(setItem)
        {}

void CommandPickUpItem  :: Activate()
{
	std::cout << "CommandPickUpItem::Activate() - Success\n";

	state = active;
}

CommandState  CommandPickUpItem  :: Process()
{
	ActivateIfInactive();

	ProcessSubCommands();

	if(owner->GetFeetPosition().getDistanceFrom(item->GetPosition()) > 1.0f
	&& !IsCurrentCommand(COMMAND_PATH_TO_POSITION))
	{
		AddSubCommand(new CommandPathToPosition(owner, item->GetPosition() + vector3df(0,0.1f,0))); // Vector is to deal with items partially submerged in ground
	}
	else
	{
		owner->AddToInventory(item->GetItem(), item->GetItemAmount());
		InstanceMgr->DestroyItemInstance(item);
		state = completed;
	}

	return state;
}

void CommandPickUpItem :: Terminate()
{
	RemoveAllSubCommands();
}

