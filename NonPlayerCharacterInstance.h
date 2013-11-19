#pragma
#ifndef NON_PLAYER_CHARACTER_INSTANCE_H
#define NON_PLAYER_CHARACTER_INSTANCE_H

#include "CharacterInstance.h"
#include "CompoundCharacterCommand.h"
struct CharacterInstanceTemplate;

class NonPlayerCharacterInstance : public CharacterInstance
{
public:
	NonPlayerCharacterInstance(CharacterInstanceTemplate* setTemplate, vector3df setPos);
	~NonPlayerCharacterInstance();

	virtual void	Update();
	virtual bool	HandleMessage(Telegram& msg);
	
	void			SetIsMoving(bool b) {isMoving = b;}
	
	void			SetDestination(vector3df dest) {destination = dest;}

private:
	virtual void UpdateMovementVelocity();
	virtual void UpdateRotation();

	// Artificial Intelligence:
	vector3df destination;
	bool isMoving;
	CompoundCharacterCommand* brain;
};
#endif
