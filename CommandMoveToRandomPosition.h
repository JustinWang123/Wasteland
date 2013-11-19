#ifndef COMMAND_RANDOM_WANDER_H
#define COMMAND_RANDOM_WANDER_H

#include "CompoundCharacterCommand.h"

class CommandMoveToRandomPosition : public CompoundCharacterCommand
{
public:
	CommandMoveToRandomPosition(CharacterInstance* owner, float maxDistance);
	~CommandMoveToRandomPosition() {}

	virtual void Activate();
	virtual CommandState  Process();
	virtual void Terminate();

protected:
	vector3df currentDestination;
	float maxDistance;
};

#endif
