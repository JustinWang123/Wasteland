#ifndef CHARACTER_INSTANCE_TEMPLATE_H
#define CHARACTER_INSTANCE_TEMPLATE_H

#include "IrrWrapper.h"
#include "Damage.h"
#include <string>

struct CharacterInstanceTemplate
{
	std::string name;
	IMesh*	    mesh;
	ITexture*   portrait;
	int		    maxFood;
	int		    maxHealth;
	float	    walkSpeed;
	float	    runSpeed;
	float	    accelerationAmount;
	Damage      unarmedDamage;
	float       unarmedDelay;
	float       unarmedRange;
};

#endif
