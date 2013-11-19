#ifndef ZOMBIE_BRAIN_H
#define ZOMBIE_BRAIN_H

#include "CompoundCharacterCommand.h"
class CharacterInstance;

class ZombieBrain : public CompoundCharacterCommand
{
public:

	ZombieBrain(CharacterInstance* owner)
		: CompoundCharacterCommand(owner, ZOMBIE_BRAIN)
	{}

	~ZombieBrain() {}

	CommandState		Process();
	void				Activate();
	void				Terminate(){}
	CharacterInstance*	GetTarget();
	virtual bool	HandleMessage(const Telegram& msg);

private:
	float thinkTime;
	static int THINK_TIME_DELAY;
};

#endif
