#ifndef DAMAGE_HIT_MESSAGE_H
#define DAMAGE_HIT_MESSAGE_H

#include "Damage.h"
#include "IrrWrapper.h"

struct DamageHitMessage
{
	Damage damage;
	vector3df position;
	vector3df heading;
};

#endif
