#ifndef COMMAND_PICK_UP_ITEM_H
#define COMMAND_PICK_UP_ITEM_H

#include "CompoundCharacterCommand.h"
class ItemInstance;

class CommandPickUpItem : public CompoundCharacterCommand
{
public:
	CommandPickUpItem(CharacterInstance* owner, ItemInstance* setItem);
	~CommandPickUpItem() {}

	virtual void Activate();
	virtual CommandState  Process();
	virtual void Terminate();

protected:
	ItemInstance* item;
};

#endif
