#ifndef COMMAND_IDLE_H
#define COMMAND_IDLE_H

#include "CharacterCommand.h"

class CommandIdle : public CharacterCommand
{
public:
	CommandIdle(CharacterInstance* owner, float idleLength);
	~CommandIdle() {}

	virtual void Activate();
	virtual CommandState  Process();
	virtual void Terminate();

protected:
	float idleStopTime;
};

#endif
