#ifndef BRAIN_SOLDIER_H
#define BRAIN_SOLDIER_H

#include "CompoundCharacterCommand.h"

class BrainSoldier : public CompoundCharacterCommand
{
public:

	BrainSoldier(CharacterInstance* owner)
		: CompoundCharacterCommand(owner, BRAIN_SOLDIER) {}

	~BrainSoldier() {}

	CommandState	Process();
	void			Activate();
	void			Terminate(){}
	virtual bool	HandleMessage(const Telegram& msg);

};

#endif
