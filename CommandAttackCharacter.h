#ifndef ATTACK_CHARACTER_H
#define ATTACK_CHARACTER_H

#include "CompoundCharacterCommand.h"
class CharacterInstance;

class CommandAttackCharacter : public CompoundCharacterCommand
{
public:
	CommandAttackCharacter(CharacterInstance* owner, CharacterInstance* setTarget);
	~CommandAttackCharacter() {}

	virtual void			Activate();
	virtual CommandState	Process();
	virtual void			Terminate();
	virtual bool			HandleMessage(const Telegram& msg);

protected:
	void FaceTarget();
	void PursueTarget();
	void AttackTarget();

	CharacterInstance*	target;
	float weaponRange;
};

#endif
