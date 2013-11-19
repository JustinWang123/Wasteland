#ifndef FACTION_MANAGER_H
#define FACTION_MANAGER_H

#include <list>
class Faction;

enum FactionType
{
    FACTION_TYPE_PLAYER_CONTROL
};

class FactionManager
{
public:
    static FactionManager* Instance();

    void Init();
    void Destroy();
    void Update();

	Faction* CreateFaction(FactionType type);

private:
    FactionManager(){}

    std::list<Faction*> factions;
};

#define FactionMgr FactionManager::Instance()

#endif
