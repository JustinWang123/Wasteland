#ifndef COMMAND_PATH_TO_CHARACTER_H
#define COMMAND_PATH_TO_CHARACTER_H

#include "CompoundCharacterCommand.h"
class CharacterInstance;

class CommandPathToCharacter : public CompoundCharacterCommand
{
public:
	CommandPathToCharacter(CharacterInstance* owner, CharacterInstance* setTarget, float setDistance);
	~CommandPathToCharacter() {}

	virtual void			Activate();
	virtual CommandState	Process();
	virtual void			Terminate();
	virtual bool			HandleMessage(const Telegram& msg);

protected:
	void					GetPathToTarget();
	CharacterInstance*		target;
	vector3df				currentPathDestination;
	float                   distance;
};

#endif
