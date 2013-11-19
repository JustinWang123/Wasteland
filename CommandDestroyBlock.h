#ifndef DESTROY_BLOCK_H
#define DESTROY_BLOCK_H

#include "CharacterCommand.h"

class CommandDestroyBlock : public CharacterCommand
{
public:
	CommandDestroyBlock(CharacterInstance* owner, vector3di setBlock, int setHotBarSlot);
	~CommandDestroyBlock() {}

	virtual void Activate();
	virtual CommandState  Process();
	virtual void Terminate();

protected:
	int			hotBarSlot;
	vector3di	block;
};

#endif
