#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

const int MAX_ITEM_DATA = 100;
const int MAX_RECIPES = 100;

#include <vector>
#include <queue>
#include <map>
#include "MapChunkInstance.h"
#include "Perameters.h"
#include "MapManager.h"
class ItemInstance;
class ZombieInstance;
class ProjectileInstance;
class ParticleInstance;
class CharacterInstance;
class Faction;

struct Recipe
{
	std::vector<int> itemIDs;
	std::vector<int> itemAmounts;
	std::vector<int> toolIDs;
	int resultID;
	int resultAmount;
};



class GameManager
{
public:
	GameManager();
	static GameManager* Instance();

	void Init(bool setCreativeModeOn);
	void Destroy();

	// Update Methods:
	void Update();

	// Global Data:
	Recipe								recipeData[MAX_RECIPES];

	// Misc Methods:
	bool								IsCreativeModeOn() {return isCreativeModeOn;}
	int									GetTime() {return time;}

private:
	bool								isCreativeModeOn;
	int									time;
};

#define GManager GameManager::Instance()
#endif
