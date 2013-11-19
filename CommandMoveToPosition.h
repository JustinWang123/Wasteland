#ifndef COMMAND_MOVE_TO_POSITION_H
#define COMMAND_MOVE_TO_POSITION_H

#include "CharacterCommand.h"

class CommandMoveToPosition : public CharacterCommand
{
public:
	CommandMoveToPosition(CharacterInstance* owner, vector3df setPosition);
	~CommandMoveToPosition() {}

	virtual void Activate();
	virtual CommandState  Process();
	virtual void Terminate();

protected:
	vector3df position;
};

#endif
