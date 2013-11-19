#ifndef COMMAND_WANDER_H
#define COMMAND_WANDER_H

#include "CompoundCharacterCommand.h"

class CommandWander : public CompoundCharacterCommand
{
public:
	CommandWander(CharacterInstance* owner);
	~CommandWander() {}

	virtual void Activate();
	virtual CommandState  Process();
	virtual void Terminate();
};

#endif
