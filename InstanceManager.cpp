#include "InstanceManager.h"
#include "BaseInstance.h"
#include "CharacterInstance.h"
#include "ProjectileInstance.h"
#include "ProjectileInstanceTemplate.h"
#include "GameManager.h"
#include "MapManager.h"
#include "CharacterInstanceTemplate.h"
#include "ItemInstance.h"
#include "ParticleInstance.h"
#include "ItemManager.h"
#include "CharacterInstanceSoldier.h"
#include "CharacterInstanceWorker.h"

InstanceManager* InstanceManager :: Instance()
{
	static InstanceManager instance;
	return &instance;
}

InstanceManager :: InstanceManager()
{
}

InstanceManager :: ~InstanceManager()
{
}

void InstanceManager :: Init()
{
	InitInstanceTemplates();

	// 0 for friendly, 1 for hostile
	// Player				// Zombie				// Animal
	factionMap[0][0] = 0;	factionMap[1][0] = 1;	factionMap[2][0] = 0; // Player

	factionMap[0][1] = 1;	factionMap[1][1] = 0;	factionMap[2][1] = 1; // Zombie

	factionMap[0][2] = 0;	factionMap[1][2] = 1;	factionMap[2][2] = 0; // Animal

}

void InstanceManager :: Destroy()
{
    for(std::list<CharacterInstance*>::iterator it = characterInstances.begin(); it != characterInstances.end(); it++)
		delete *it;

	for(std::list<ProjectileInstance*>::iterator it = projectileInstances.begin(); it != projectileInstances.end(); it++)
		delete *it;

    for(std::vector<ItemInstance*>::iterator it = itemInstances.begin(); it != itemInstances.end(); it++)
		delete *it;

	for(std::map<int, ProjectileInstanceTemplate*>::iterator it = projectileInstanceTemplates.begin(); it != projectileInstanceTemplates.end(); it++)
		delete (*it).second;

	for(std::map<int, CharacterInstanceTemplate*>::iterator it = characterInstanceTemplates.begin(); it != characterInstanceTemplates.end(); it++)
		delete (*it).second;
}

void InstanceManager :: InitInstanceTemplates()
{
	// ----------------------------------------------------------------------------------------
	// Projectile Instance Templates:
	// ----------------------------------------------------------------------------------------
	// Bullet:
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BULLET] = new ProjectileInstanceTemplate();
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BULLET]->name = "Bullet";
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BULLET]->type = PROJECTILE_TYPE_BULLET;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BULLET]->lifeTime = 5.0f;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BULLET]->speed = 50.0f; // 1000
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BULLET]->gravityAcc = 0;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BULLET]->size = 0.1; // 0.01
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BULLET]->color.setRed(255);
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BULLET]->color.setGreen(255);
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BULLET]->color.setBlue(255);

	// Blood:
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BLOOD] = new ProjectileInstanceTemplate();
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BLOOD]->name = "Blood";
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BLOOD]->type = PROJECTILE_TYPE_PARTICLE;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BLOOD]->lifeTime = 1.0f;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BLOOD]->speed = 3.0f;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BLOOD]->gravityAcc = 6.0f;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BLOOD]->size = 0.1;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BLOOD]->color.setRed(255);
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BLOOD]->color.setGreen(0);
	projectileInstanceTemplates[PROJECTILE_INSTANCE_BLOOD]->color.setBlue(0);

	// Concrete Dust:
	projectileInstanceTemplates[PROJECTILE_INSTANCE_DUST] = new ProjectileInstanceTemplate();
	projectileInstanceTemplates[PROJECTILE_INSTANCE_DUST]->name = "Concrete Dust";
	projectileInstanceTemplates[PROJECTILE_INSTANCE_DUST]->type = PROJECTILE_TYPE_PARTICLE;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_DUST]->lifeTime = 1.0f;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_DUST]->speed = 3.0f;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_DUST]->gravityAcc = 6.0f;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_DUST]->size = 0.075;
	projectileInstanceTemplates[PROJECTILE_INSTANCE_DUST]->color.setRed(200);
	projectileInstanceTemplates[PROJECTILE_INSTANCE_DUST]->color.setGreen(200);
	projectileInstanceTemplates[PROJECTILE_INSTANCE_DUST]->color.setBlue(200);

	// ----------------------------------------------------------------------------------------
	// Character Instance Templates:
	// ----------------------------------------------------------------------------------------
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER] = new CharacterInstanceTemplate();
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->name = "Soldier";
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->mesh = IRR->sceneMgr->getMesh("mdl/Human.obj");
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->portrait = IRR->driver->getTexture("tex/PlayerPortrait.bmp");
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->maxFood = 100;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->maxHealth = 100;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->runSpeed = 3.0f;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->walkSpeed = 2.0f;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->accelerationAmount = 100.0f;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->unarmedDamage.type[DAMAGE_TYPE_FLESH] = 10;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->unarmedDamage.type[DAMAGE_TYPE_METAL] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->unarmedDamage.type[DAMAGE_TYPE_SOIL] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->unarmedDamage.type[DAMAGE_TYPE_STONE] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->unarmedDamage.type[DAMAGE_TYPE_WOOD] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->unarmedRange = 1.5f;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_SOLDIER]->unarmedDelay = 0.5f;

	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER] = new CharacterInstanceTemplate();
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->name = "Worker";
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->mesh = IRR->sceneMgr->getMesh("mdl/Human.obj");
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->portrait = IRR->driver->getTexture("tex/PlayerPortrait.bmp");
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->maxFood = 100;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->maxHealth = 100;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->runSpeed = 3.0f;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->walkSpeed = 2.0f;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->accelerationAmount = 100.0f;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->unarmedDamage.type[DAMAGE_TYPE_FLESH] = 10;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->unarmedDamage.type[DAMAGE_TYPE_METAL] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->unarmedDamage.type[DAMAGE_TYPE_SOIL] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->unarmedDamage.type[DAMAGE_TYPE_STONE] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->unarmedDamage.type[DAMAGE_TYPE_WOOD] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->unarmedRange = 1.5f;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER]->unarmedDelay = 0.5f;


	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE] = new CharacterInstanceTemplate();
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->name = "Zombie";
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->mesh = IRR->sceneMgr->getMesh("mdl/Human.obj");
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->portrait = IRR->driver->getTexture("tex/PlayerPortrait.bmp");
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->maxFood = 100;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->maxHealth = 100;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->walkSpeed = 1.5f;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->runSpeed = 3.0f;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->accelerationAmount = 100.0f;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->unarmedDamage.type[DAMAGE_TYPE_FLESH] = 10;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->unarmedDamage.type[DAMAGE_TYPE_METAL] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->unarmedDamage.type[DAMAGE_TYPE_SOIL] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->unarmedDamage.type[DAMAGE_TYPE_STONE] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->unarmedDamage.type[DAMAGE_TYPE_WOOD] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->unarmedRange = 1.5f;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_ZOMBIE]->unarmedDelay = 0.5f;

	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW] = new CharacterInstanceTemplate();
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->name = "Cow";
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->mesh = IRR->sceneMgr->getMesh("mdl/Cow.obj");
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->portrait = IRR->driver->getTexture("tex/PlayerPortrait.bmp");
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->maxFood = 100;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->maxHealth = 100;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->walkSpeed = 1.5f;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->runSpeed = 3.0f;
	characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->accelerationAmount = 100.0f;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->unarmedDamage.type[DAMAGE_TYPE_FLESH] = 10;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->unarmedDamage.type[DAMAGE_TYPE_METAL] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->unarmedDamage.type[DAMAGE_TYPE_SOIL] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->unarmedDamage.type[DAMAGE_TYPE_STONE] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->unarmedDamage.type[DAMAGE_TYPE_WOOD] = 0;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->unarmedRange = 1.0f;
    characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_COW]->unarmedDelay = 0.5f;
}

void InstanceManager :: Update()
{
	// Update projectiles:
	for(std::list<ProjectileInstance*>::iterator it = projectileInstances.begin(); it != projectileInstances.end(); /**/)
	{
		// Update if alive:
		if(!(*it)->GetRemoveMe())
		{
			(*it)->Update();
			++it;
		}
		else
		{
			delete *it;
			it = projectileInstances.erase(it);
		}
	}

	// Update Characters
	for(std::list<CharacterInstance*>::iterator it = characterInstances.begin(); it != characterInstances.end(); )
	{
		// Update if alive:
		if(!(*it)->RemoveMe())
		{
			// Only update zombies in normal game mode:
			if(!GManager->IsCreativeModeOn())
			{
				(*it)->Update();
			}

			++it;
		}
		else
		{
			delete *it;
			it = characterInstances.erase(it);
		}
	}

	// Update Items Instances:
	for(std::vector<ItemInstance*>::iterator it = itemInstances.begin(); it != itemInstances.end(); ++it)
	{
		(*it)->Update();
	}
}

// ----------------------------------------------------------------------------------------
// Creation of new instances:
// ----------------------------------------------------------------------------------------
ProjectileInstance* InstanceManager :: CreateProjectileInstance(int projectileInstanceTemplateID, vector3df pos, vector3df heading, CharacterInstance* setOwner)
{
	ProjectileInstance* projectile = new ProjectileInstance(projectileInstanceTemplates[projectileInstanceTemplateID], pos, heading, setOwner);
	projectileInstances.push_back(projectile);
	return projectile;
}

ProjectileInstance* InstanceManager :: CreateProjectileInstance(int projectileInstanceTemplateID, Damage* setDamage, vector3df pos, vector3df heading, CharacterInstance* setOwner)
{
	ProjectileInstance* projectile = new ProjectileInstance(projectileInstanceTemplates[projectileInstanceTemplateID], setDamage, pos, heading, setOwner);
	projectileInstances.push_back(projectile);
	return projectile;
}

CharacterInstanceSoldier* InstanceManager :: CreateCharacterInstanceSoldier(int characterInstanceTemplateID, vector3df pos)
{
    CharacterInstanceSoldier* character = new CharacterInstanceSoldier(characterInstanceTemplates[characterInstanceTemplateID], pos);
    characterInstances.push_back(character);
    return character;
}

CharacterInstanceWorker* InstanceManager :: CreateCharacterInstanceWorker(vector3df pos)
{
	CharacterInstanceWorker* character = new CharacterInstanceWorker(characterInstanceTemplates[CHARACTER_INSTANCE_TEMPLATE_WORKER], pos);
    characterInstances.push_back(character);
    return character;
}

void InstanceManager :: CreateItemInstance(int itemTemplateID, int itemAmount, vector3df pos, vector3df rot)
{
	// Only create items from existing templates:
	if(ItemMgr->ItemTemplateExists(itemTemplateID))
	{
		CreateItemInstance(ItemMgr->CreateItem(itemTemplateID), itemAmount, pos, rot);
	}
}

void InstanceManager :: CreateItemInstance(BaseItem* pItem, int itemAmount, vector3df pos, vector3df rot)
{
	itemInstances.push_back(new ItemInstance(pItem, itemAmount, pos, rot));
}

void InstanceManager :: DestroyItemInstance(ItemInstance* item)
{
	for(std::vector<ItemInstance*>::iterator it = itemInstances.begin(); it != itemInstances.end(); it++)
	{
		if( (*it) == item)
		{
			itemInstances.erase(it);
			break;
		}
	}

	delete item;
}


// ----------------------------------------------------------------------------------------
// Management of instances by id:
// ----------------------------------------------------------------------------------------
BaseInstance* InstanceManager :: GetInstanceFromID(int id) const
{
  //find the entity
  std::map<int, BaseInstance*> :: const_iterator instance = instanceMap.find(id);

  return instance->second;
}

CharacterInstance*	InstanceManager :: GetCharacterFromID(int id) const
{
	return ((CharacterInstance*)GetInstanceFromID(id));
}

BaseInstance* InstanceManager :: GetInstanceFromSceneNode(ISceneNode* sceneNode)
{
	for(std::map<int, BaseInstance*>::iterator it = instanceMap.begin(); it != instanceMap.end(); it++)
	{
		if( (*it).second->GetSceneNode() == sceneNode)
		{
			return (*it).second;
		}
	}

	return 0;
}

void InstanceManager :: Remove(BaseInstance* i)
{
  instanceMap.erase( instanceMap.find(i->GetID()) );
}

void  InstanceManager :: Register(BaseInstance* i)
{
  instanceMap.insert(std::make_pair(i->GetID(), i));
}




// ----------------------------------------------------------------------------------------
// // Get lists of instance ids based on certain perameters:
// ----------------------------------------------------------------------------------------
bool InstanceManager :: IsPosSolid(vector3df pos, int bitMask)
{
	int instanceID;

	return IsPosSolid(pos, bitMask, instanceID);
}

bool InstanceManager :: IsPosSolid(vector3df pos, int bitMask, int &instanceID)
{
	if(bitMask & BITMASK_WORLD
	&& MapMgr->IsBlockSolid(pos))
	{
		instanceID = MapMgr->GetChunkInstanceID(pos);
		return true;
	}

	if(bitMask & BITMASK_CHARACTER)
	{
		for(std::list<CharacterInstance*>::iterator it = characterInstances.begin(); it != characterInstances.end(); it++)
			if( (*it)->IsPosSolid(pos) )
			{
				instanceID = (*it)->GetID();
				return true;
			}
	}

	return false;
}

bool InstanceManager :: IsRaySolid(vector3df rayStart, vector3df rayHeading, float rayLength, int bitMask)
{
	int instanceID;
	vector3df impactPos;

	return IsRaySolid(rayStart, rayHeading, rayLength, bitMask, instanceID, impactPos);
}

bool InstanceManager :: IsRaySolid(vector3df rayStart, vector3df rayEnd, int bitMask)
{
	int instanceID;
	vector3df impactPos;

	return IsRaySolid(rayStart, rayEnd, bitMask, instanceID, impactPos);
}

bool InstanceManager :: IsRaySolid(vector3df rayStart, vector3df rayEnd, int bitMask, int &instanceID, vector3df &impactPos)
{
	vector3df heading = rayEnd - rayStart;
	heading.normalize();

	return IsRaySolid(rayStart, heading, rayStart.getDistanceFrom(rayEnd), bitMask, instanceID, impactPos);
}


bool InstanceManager :: IsRaySolid(vector3df rayStart, vector3df rayHeading, float rayLength, int bitMask, int &instanceID, vector3df &impactPos)
{
	instanceID = -1;

	if(bitMask & BITMASK_CHARACTER || bitMask & BITMASK_ITEM)
	{
		line3d<f32> ray;
		triangle3df hitTriangle;
		ray.start = rayStart;
		ray.end = rayStart + (rayHeading * rayLength);

		ISceneNode* node = 0;

		//node = IRR->collisionMgr->getSceneNodeAndCollisionPointFromRay(ray, impactPos, hitTriangle, bitMask & (BITMASK_CHARACTER | BITMASK_ITEM));
        node = IRR->collisionMgr->getSceneNodeFromRayBB(ray, bitMask & (BITMASK_CHARACTER | BITMASK_ITEM));

		if(node)
		{
            BaseInstance* instance = GetInstanceFromSceneNode(node);
			instanceID = instance->GetID();
			impactPos = instance->GetPosition();
		}
	}

	if(bitMask & BITMASK_WORLD)
	{
		vector3df worldImpactPos;

		// If collision has already occured then we need to determine if the map collision is closer:
		if(instanceID != -1)
		{
			if(MapMgr->GetPointOfMapCollision(rayStart, rayHeading, rayLength, worldImpactPos)
			&& worldImpactPos.getDistanceFromSQ(rayStart) < impactPos.getDistanceFromSQ(rayStart))
			{
				instanceID = MapMgr->GetChunkInstanceID(vector3df(worldImpactPos.X, worldImpactPos.Y, worldImpactPos.Z));
				impactPos = worldImpactPos;
			}
		}
		else
		{
			if(MapMgr->GetPointOfMapCollision(rayStart, rayHeading, rayLength, worldImpactPos))
			{
				instanceID = MapMgr->GetChunkInstanceID(vector3df(worldImpactPos.X, worldImpactPos.Y, worldImpactPos.Z));
				impactPos = worldImpactPos;
			}
		}
	}

	return instanceID != -1;
}

bool InstanceManager :: GetCharactersInBox(std::list<int> &list, vector3df smallCornerPos, vector3df largeCornerPos)
{
	for(std::list<CharacterInstance*>::iterator it = characterInstances.begin(); it != characterInstances.end(); it++)
		if((*it)->GetFeetPosition().X >= smallCornerPos.X
		&& (*it)->GetFeetPosition().Y >= smallCornerPos.Y
		&& (*it)->GetFeetPosition().Z >= smallCornerPos.Z
		&& (*it)->GetFeetPosition().X < largeCornerPos.X
		&& (*it)->GetFeetPosition().Y < largeCornerPos.Y
		&& (*it)->GetFeetPosition().Z < largeCornerPos.Z)
		{
			list.push_back( (*it)->GetID() );
		}

	if(list.size() > 0)
		return true;
	else
		return false;
}

bool InstanceManager :: GetCharactersInRadius(std::list<int> &list, vector3df center, float radius)
{
	for(std::list<CharacterInstance*>::iterator it = characterInstances.begin(); it != characterInstances.end(); it++)
		if((*it)->GetFeetPosition().getDistanceFrom(center) < radius)
		{
			list.push_back( (*it)->GetID() );
		}

	if(list.size() > 0)
		return true;
	else
		return false;
}

bool InstanceManager :: GetChunkAtPoint(int &chunkID, vector3df point)
{
	if(MapMgr->IsBlockSolid(point))
	{
		chunkID = MapMgr->GetChunkInstanceID(point);
		return true;
	}
	else
	{
		chunkID = -1;
		return false;
	}
}

bool InstanceManager :: GetFirstBlockHitByRay(int &chunkID, vector3di &blockIndex, vector3df position, vector3df heading, float distance)
{
	vector3di closedBlock;

	if( MapMgr->GetFirstClosedBlock(position, heading, distance, closedBlock))
	{
		chunkID = MapMgr->GetChunkInstanceID(vector3df(closedBlock.X, closedBlock.Y, closedBlock.Z));
		blockIndex = closedBlock;
		return true;
	}
	else
	{
		chunkID = -1;
		return false;
	}
}

bool InstanceManager :: GetChunkPosHitByRay(int &chunkID, vector3df &impactPos, vector3df position, vector3df heading, float distance)
{
	heading.normalize();

	if(MapMgr->GetPointOfMapCollision(position, heading, distance, impactPos))
	{
		chunkID = MapMgr->GetChunkInstanceID(vector3df(impactPos.X, impactPos.Y, impactPos.Z));
		return true;
	}
	else
	{
		chunkID = -1;
		return false;
	}
}

// --------------------------------------------------------------------------------
// Faction:
// --------------------------------------------------------------------------------
bool InstanceManager :: IsHostile(int firstFactionID, int secondFactionID)
{
	if(factionMap[firstFactionID][secondFactionID] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
