#ifndef PROJECTILE_INSTANCE_H
#define PROJECTILE_INSTANCE_H

#include "IrrWrapper.h"
#include "Damage.h"
class CharacterInstance;
struct ProjectileInstanceTemplate;

class ProjectileInstance
{
public:
	ProjectileInstance(ProjectileInstanceTemplate* setTemplate, vector3df setPos, vector3df setHeading, CharacterInstance* setOwner);
	ProjectileInstance(ProjectileInstanceTemplate* setTemplate, Damage* setDamage, vector3df setPos, vector3df setHeading, CharacterInstance* setOwner);
	~ProjectileInstance();

	void Update();

	vector3df 	GetPosition() const {return position;}
	vector3df 	GetHeading() const {return heading;}
	Damage* 	GetDamage();
	bool GetRemoveMe() const {return removeMe;}

private:
	IBillboardSceneNode* sceneNode;
	vector3df	position;
	vector3df	velocity;
	vector3df	heading;

	bool removeMe;

	float life;

	CharacterInstance* owner;

	ProjectileInstanceTemplate* pTemplate;
	Damage* damage;
};
#endif
