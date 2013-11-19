#include "ProjectileInstance.h"
#include "Perameters.h"
#include "GameManager.h"
#include "ZombieInstance.h"
#include "CharacterInstance.h"
#include "InstanceManager.h"
#include "MessageManager.h"
#include "ProjectileInstanceTemplate.h"
#include "Damage.h"

ProjectileInstance :: ProjectileInstance(ProjectileInstanceTemplate* setTemplate, vector3df setPos, vector3df setHeading, CharacterInstance* setOwner)
	: 	sceneNode(0),
		position(setPos),
		velocity(setHeading * setTemplate->speed),
		heading(setHeading.normalize()),
		removeMe(false),
		life(setTemplate->lifeTime),
		owner(setOwner),
		pTemplate(setTemplate),
		damage(0)
{
	sceneNode = IRR->sceneMgr->addBillboardSceneNode(0, dimension2d<f32>(pTemplate->size, pTemplate->size), setPos, 0);
	sceneNode->setMaterialFlag(EMF_LIGHTING, false);
	sceneNode->setColor(pTemplate->color);
}

ProjectileInstance :: ProjectileInstance(ProjectileInstanceTemplate* setTemplate, Damage* setDamage, vector3df setPos, vector3df setHeading, CharacterInstance* setOwner)
	: 	sceneNode(0),
		position(setPos),
		velocity(setHeading * setTemplate->speed),
		heading(setHeading.normalize()),
		removeMe(false),
		life(setTemplate->lifeTime),
		owner(setOwner),
		pTemplate(setTemplate),
		damage(setDamage)
{
	sceneNode = IRR->sceneMgr->addBillboardSceneNode(0, dimension2d<f32>(pTemplate->size, pTemplate->size), setPos, 0);
	sceneNode->setMaterialFlag(EMF_LIGHTING, false);
	sceneNode->setColor(pTemplate->color);
}

ProjectileInstance :: ~ProjectileInstance()
{
	if(sceneNode)
		sceneNode->remove();
}

void ProjectileInstance :: Update()
{
	// Check collision with characters:
	int instanceID;
	vector3df impactPos;

	if(pTemplate->type == PROJECTILE_TYPE_BULLET
	&& InstanceMgr->IsRaySolid(position, heading,  pTemplate->speed * IRR->GetTimeDelta(), BITMASK_CHARACTER | BITMASK_WORLD, instanceID, impactPos)
	&& (owner == 0 || instanceID != owner->GetID()))
	{
		DamageHitMessage message;
		message.damage = *damage;
		message.position = impactPos;
		message.heading = heading;

		MessageMgr->DispatchMsg(0.0f, -1, instanceID, MESSAGE_TYPE_DAMAGE_HIT, (void*)&message);
		removeMe = true;
	}

	// Update position otherwise:
	velocity -= vector3df(0, pTemplate->gravityAcc, 0) * IRR->GetTimeDelta();
	position += velocity * IRR->GetTimeDelta();

	life -= IRR->GetTimeDelta();

	if(life <= 0)
		removeMe = true;

	if(sceneNode)
		sceneNode->setPosition(position);
}

Damage* ProjectileInstance :: GetDamage()
{
	return damage;
}
