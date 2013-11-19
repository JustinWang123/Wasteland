#ifndef CHARACTER_COMMAND_H
#define CHARACTER_COMMAND_H

#include "IrrWrapper.h"
class CharacterInstance;
struct Telegram;

enum CommandState
{
	active,
	inactive,
	completed,
	failed
};


enum CommandType
{
	BRAIN_NON_FACTION_DOCILE,
	BRAIN_SOLDIER,
	BRAIN_WORKER,
	BRAIN_NON_FACTION_HOSTILE,
	COMMAND_MOVE_TO_POSITION,
	COMMAND_PATH_TO_POSITION,
	COMMAND_ATTACK_CHARACTER,
	COMMAND_PATH_TO_CHARACTER,
	COMMAND_IDLE,
	COMMAND_WANDER,
	COMMAND_PICK_UP_ITEM,
	COMMAND_HARVEST_BLOCK
};

class CharacterCommand
{
public:
	CharacterCommand(CharacterInstance* setOwner, CommandType setType);
	virtual ~CharacterCommand();

	void 					SetOwner(CharacterInstance* setOwner) {owner = setOwner;}
	virtual void			Activate() {}
	virtual CommandState	Process() {return inactive;}
	virtual void			Terminate() {}

	void					ReactivateIfFailed();
	void					ActivateIfInactive();

	//goals can handle messages. Many don't though, so this defines a default behavior
	virtual bool			HandleMessage(const Telegram& msg) {return false;}


	// Accessors:
	virtual bool	IsEqual(CharacterCommand* command){return false;}
	bool         	IsComplete() const {return state == completed;}
	bool         	IsActive() const {return state == active;}
	bool         	IsInactive() const {return state == inactive;}
	bool         	HasFailed() const {return state == failed;}
	CommandType  	GetType() const {return type;}

protected:
	CommandType type;
	CommandState state;

	CharacterInstance* owner;
	ISceneNode* debugObject;

};

#endif
