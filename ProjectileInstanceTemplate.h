#ifndef PROJECTILE_INSTANCE_TEMPLATE_H
#define PROJECTILE_INSTANCE_TEMPLATE_H

#include "IrrWrapper.h"
#include "Damage.h"
#include <string>

enum ProjectileType
{
	PROJECTILE_TYPE_BULLET,
	PROJECTILE_TYPE_PARTICLE
};

struct ProjectileInstanceTemplate
{
	std::string name;
	ProjectileType type;
	float size;
	float speed;
	float gravityAcc;
	int lifeTime;
	SColor color;
};

#endif
