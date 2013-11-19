#ifndef COMMAND_PATH_TO_POSITION_H
#define COMMAND_PATH_TO_POSITION_H

#include "CompoundCharacterCommand.h"

class CommandPathToPosition : public CompoundCharacterCommand
{
public:
	CommandPathToPosition(CharacterInstance* owner, vector3df setPosition);
	~CommandPathToPosition() {}

	virtual void Activate();
	virtual CommandState  Process();
	virtual void Terminate();

protected:
	vector3df position;
};

#endif
