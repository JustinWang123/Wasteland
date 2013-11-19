#ifndef FACTION_H
#define FACTION_H

#include <list>
class CharacterInstanceSoldier;
class CharacterInstanceWorker;
class CharacterCommand;

class Faction
{
public:
    Faction();
    virtual ~Faction();

    virtual void Update()=0;

    void AddSoldier(CharacterInstanceSoldier* soldier);
    void AddWorker(CharacterInstanceWorker* worker);
    int GetFactionID() const {return factionID;}

protected:
	void AddCommand(CharacterCommand* command);

    std::list<CharacterInstanceSoldier*> 	soldierInstances;
	std::list<CharacterInstanceWorker*> 	workerInstances;
	std::list<CharacterCommand*> 			commandQueue;

    static int nextFactionID;
    int factionID;
};

#endif
