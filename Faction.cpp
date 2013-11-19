#include "Faction.h"
#include "CharacterCommand.h"

int Faction :: nextFactionID = 0;

Faction :: Faction()
{
    factionID = nextFactionID;

    factionID++;
}

Faction :: ~Faction()
{
	for(std::list<CharacterCommand*>::iterator it = commandQueue.begin(); it != commandQueue.end(); it++)
	{
		delete *it;
	}
}

void Faction :: Update()
{
}

void Faction :: AddSoldier(CharacterInstanceSoldier* soldier)
{
    soldierInstances.push_back(soldier);
}

void Faction :: AddWorker(CharacterInstanceWorker* worker)
{
	workerInstances.push_back(worker);
}

void Faction :: AddCommand(CharacterCommand* command)
{
	bool commandExistsAlready = false;

	// Determine if command is already on the stack
	for(std::list<CharacterCommand*>::iterator it = commandQueue.begin(); it != commandQueue.end(); it++)
	{
		if( (*it)->IsEqual(command) )
		{
			commandExistsAlready = true;
			break;
		}
	}

	if(!commandExistsAlready)
	{
		commandQueue.push_back(command);
	}
	else
	{
		delete command;
	}
}
