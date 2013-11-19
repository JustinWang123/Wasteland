#ifndef COMMAND_HARVEST_BLOCK_H
#define COMMAND_HARVEST_BLOCK_H

#include "CompoundCharacterCommand.h"
class CharacterInstance;

class CommandHarvestBlock : public CompoundCharacterCommand
{
public:
	CommandHarvestBlock(vector3di setTargetBlock);
	~CommandHarvestBlock() {}

	virtual bool			IsEqual(CharacterCommand* command);
	vector3di				GetTargetBlock() {return targetBlock;}
	virtual void			Activate();
	virtual CommandState	Process();
	virtual void			Terminate();
	virtual bool			HandleMessage(const Telegram& msg);

protected:

	vector3di targetBlock;
	vector3df targetBlockPos;
};

#endif
