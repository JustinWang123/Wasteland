#ifndef INSTANCE_MANAGER_H
#define INSTANCE_MANAGER_H

#include "IrrWrapper.h"
#include <map>
#include <list>
#include <vector>

class ProjectileInstance;
struct ProjectileInstanceTemplate;
class CharacterInstance;
class BaseInstance;
class PlayerInstance;
class CharacterInstance;
struct CharacterInstanceTemplate;
class BaseItem;
class ItemInstance;
class ParticleInstance;
class CharacterInstanceSoldier;
class CharacterInstanceWorker;
struct Damage;

const int NUM_OF_FACTIONS = 10;

enum CharacterType
{
    CHARACTER_TYPE_SOLDIER
};

class InstanceManager
{
public:
	static InstanceManager*         Instance();
                                    ~InstanceManager();

	void                            Init();
	void                            Update();
	void                            Destroy();

	// Creation of new instances:
	ProjectileInstance*				CreateProjectileInstance(int projectileInstanceTemplateID, vector3df pos, vector3df heading, CharacterInstance* setOwner);
	ProjectileInstance*				CreateProjectileInstance(int projectileInstanceTemplateID, Damage* setDamage, vector3df pos, vector3df heading, CharacterInstance* setOwner);
	CharacterInstanceSoldier*       CreateCharacterInstanceSoldier(int characterInstanceTemplateID, vector3df pos);
    CharacterInstanceWorker*       	CreateCharacterInstanceWorker(vector3df pos);
    void			                CreateItemInstance(int itemTemplateID, int itemAmount, vector3df pos, vector3df rot);
	void			                CreateItemInstance(BaseItem* pItem, int itemAmount, vector3df pos, vector3df rot);

	// Destruction of instances:
	void							DestroyItemInstance(ItemInstance* item);

	// Management of instances by id:
	void							Register(BaseInstance* i);
	void							Remove(BaseInstance* i);

	// Retreive instances from instanceIDs:
	BaseInstance*					GetInstanceFromID(int id) const;
	CharacterInstance*				GetCharacterFromID(int id) const;

    // Possible Bullshit:
    std::vector<ItemInstance*>*     GetItemInstances() {return &itemInstances;}

	// Retreive instnaces from sceneNodes:
	BaseInstance*					GetInstanceFromSceneNode(ISceneNode* sceneNode);

	// Collision detection:
	bool IsPosSolid(vector3df pos, int bitMask);
	bool IsPosSolid(vector3df pos, int bitMask, int &instanceID);

	bool IsRaySolid(vector3df rayStart, vector3df rayHeading, float rayLength, int bitMask);
	bool IsRaySolid(vector3df rayStart, vector3df rayHeading, float rayLength, int bitMask, int &instanceID, vector3df &impactPos);
	bool IsRaySolid(vector3df rayStart, vector3df rayEnd, int bitMask);
	bool IsRaySolid(vector3df rayStart, vector3df rayEnd, int bitMask, int &instanceID, vector3df &impactPos);

	bool GetCharactersInBox(std::list<int> &list, vector3df smallCornerPos, vector3df largeCornerPos);
	bool GetCharactersInRadius(std::list<int> &list, vector3df center, float radius);
	bool GetChunkAtPoint(int &chunkID, vector3df point);
	bool GetChunkPosHitByRay(int &chunkID, vector3df &impactPos, vector3df position, vector3df heading, float distance);
	bool GetFirstBlockHitByRay(int &chunkID, vector3di &blockIndex, vector3df position, vector3df heading, float distance);

	// Faction:
	bool IsHostile(int firstFactionID, int secondFactionID);

private:
	InstanceManager();
	void InitInstanceTemplates();

	// Instance Map:
	std::map<int, BaseInstance*>    instanceMap;

	// Instance Lists and Vectors:
	std::list<ProjectileInstance*>	projectileInstances;
	std::list<CharacterInstance*>	characterInstances;
	std::vector<ItemInstance*>		itemInstances;

	// Instance Templates:
	std::map<int, ProjectileInstanceTemplate*> projectileInstanceTemplates;
	std::map<int, CharacterInstanceTemplate*> characterInstanceTemplates;

	// Faction Map:
	int factionMap[NUM_OF_FACTIONS][NUM_OF_FACTIONS];
};

#define InstanceMgr InstanceManager::Instance()
#endif
