#ifndef BRAIN_NON_FACTION_HOSTILE_H
#define BRAIN_NON_FACTION_HOSTILE_H

#include "CompoundCharacterCommand.h"
class CharacterInstanceSoldier;

class BrainNonFactionHostile : public CompoundCharacterCommand
{
public:

	BrainNonFactionHostile(CharacterInstanceSoldier* owner)
		: CompoundCharacterCommand((CharacterInstance*)owner, BRAIN_NON_FACTION_HOSTILE)
	{}

	~BrainNonFactionHostile() {}

	CommandState		Process();
	void				Activate();
	void				Terminate(){}
	CharacterInstance*	GetAgroTarget();
	virtual bool	HandleMessage(const Telegram& msg);

private:
	float thinkTime;
	static int THINK_TIME_DELAY;
};

#endif
