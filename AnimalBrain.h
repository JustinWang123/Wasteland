#ifndef BRAIN_NON_FACTION_DOCILE_H
#define BRAIN_NON_FACTION_DOCILE_H

#include "CompoundCharacterCommand.h"

class AnimalBrain : public CompoundCharacterCommand
{
public:

	AnimalBrain(CharacterInstance* owner)
		: CompoundCharacterCommand(owner, BRAIN_NON_FACTION_DOCILE)
	{}

	~AnimalBrain() {}

	CommandState	Process();
	void			Activate();
	void			Terminate(){}
	virtual bool	HandleMessage(const Telegram& msg);

private:
	float thinkTime;
	static int THINK_TIME_DELAY;

};

#endif
