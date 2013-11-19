#include "FactionManager.h"
#include "FactionPlayer.h"
#include "Faction.h"


FactionManager* FactionManager :: Instance()
{
    static FactionManager instance;
    return &instance;
}

void FactionManager :: Init()
{
}

void FactionManager :: Destroy()
{
    for(std::list<Faction*>::iterator it = factions.begin(); it != factions.end(); it++)
    {
        delete *it;
    }

    factions.clear();
}

void FactionManager :: Update()
{
    for(std::list<Faction*>::iterator it = factions.begin(); it != factions.end(); it++)
    {
        (*it)->Update();
    }
}

Faction* FactionManager :: CreateFaction(FactionType type)
{
    Faction* newFaction = 0;

    if(type == FACTION_TYPE_PLAYER_CONTROL)
    {
        newFaction = new FactionPlayer();
        factions.push_back(newFaction);
    }

    return newFaction;
}
