#ifndef COMPOUND_CHARACTER_COMMAND
#define COMPOUND_CHARACTER_COMMAND

#include "CharacterCommand.h"
#include <list>

class CompoundCharacterCommand : public CharacterCommand
{
public:

	CompoundCharacterCommand(CharacterInstance* setOwner, CommandType setType) : CharacterCommand(setOwner,  setType) {}

	virtual ~CompoundCharacterCommand() { RemoveAllSubCommands(); }

	virtual void Activate() = 0;
	virtual CommandState  Process() = 0;
	virtual void Terminate() = 0;

	void AddSubCommand(CharacterCommand* c);
	void RemoveAllSubCommands();
	bool IsCurrentCommand(CharacterCommand* c);
	bool IsCurrentCommand(CommandType type);

protected:
	CommandState  ProcessSubCommands();

	std::list<CharacterCommand*>	subCommandList;
};
#endif
