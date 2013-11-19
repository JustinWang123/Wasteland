#include "NonPlayerCharacterInstance.h"
#include "ZombieBrain.h"
#include "AnimalBrain.h"
#include "CharacterInstanceTemplate.h"

NonPlayerCharacterInstance :: NonPlayerCharacterInstance(CharacterInstanceTemplate* setTemplate, vector3df setPos)
	: CharacterInstance(setTemplate, setPos)
{
	isMoving = false;
	

	if(setTemplate->brainType == BRAIN_TYPE_ZOMBIE)
	{
		brain = new ZombieBrain(this);
	}
	else if(setTemplate->brainType == BRAIN_TYPE_ANIMAL)
	{
		brain = new AnimalBrain(this);
	}
}

NonPlayerCharacterInstance :: ~NonPlayerCharacterInstance()
{
}

void NonPlayerCharacterInstance :: Update()
{
	if(!isDead)
	{
		brain->Process();

		UpdateMovementVelocity();
		UpdateRotation();
	}

	CharacterInstance::Update();
}

bool NonPlayerCharacterInstance :: HandleMessage(Telegram& msg)
{
	if(brain->HandleMessage(msg))
	{
		return true;
	}

	if(CharacterInstance::HandleMessage(msg))
	{
		return true;
	}

	return false;
}
	
void NonPlayerCharacterInstance :: UpdateMovementVelocity()
{
	if(isMoving)
	{
		movementDir = destination - GetFeetPosition();
		movementDir.Y = 0;
		movementDir.normalize();
	}
	else
	{
		movementDir = vector3df(0,0,0);
	}
}

void NonPlayerCharacterInstance :: UpdateRotation()
{
}