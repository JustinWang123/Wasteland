#pragma
#ifndef PLACE_BLOCK_H
#define PLACE_BLOCK_H

#include "CharacterCommand.h"

class CommandPlaceBlock : public CharacterCommand
{
public:
	CommandPlaceBlock(NonPlayerCharacterInstance* owner, vector3di setBlock, int setHotBarSlot);
	~CommandPlaceBlock() {}

	virtual void Activate();
	virtual CommandState  Process();
	virtual void Terminate();

protected:
	int			hotBarSlot;
	vector3di	block;
};

#endif
