#ifndef DAMAGE_H
#define DAMAGE_H

enum DamageType
{
	DAMAGE_TYPE_FLESH,
	DAMAGE_TYPE_STONE,
	DAMAGE_TYPE_METAL,
	DAMAGE_TYPE_WOOD,
	DAMAGE_TYPE_SOIL
};

struct Damage
{
	Damage()
	{
		type[DAMAGE_TYPE_FLESH] = 0;
		type[DAMAGE_TYPE_STONE] = 0;
		type[DAMAGE_TYPE_METAL] = 0;
		type[DAMAGE_TYPE_WOOD] = 0;
		type[DAMAGE_TYPE_SOIL] = 0;
	}

	int type[5];
};

#endif