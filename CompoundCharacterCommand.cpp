#include "CompoundCharacterCommand.h"



CommandState CompoundCharacterCommand :: ProcessSubCommands()
{ 
	//remove all completed and failed goals from the front of the subgoal list
	while (!subCommandList.empty() &&
	(subCommandList.front()->IsComplete() || subCommandList.front()->HasFailed()))
	{    
		subCommandList.front()->Terminate();
		delete subCommandList.front(); 
		subCommandList.pop_front();
	}

	//if any subgoals remain, process the one at the front of the list
	if (!subCommandList.empty())
	{ 
		//grab the status of the front-most subgoal
		CommandState subCommandState = subCommandList.front()->Process();

		//we have to test for the special case where the front-most subgoal
		//reports 'completed' *and* the subgoal list contains additional goals.When
		//this is the case, to ensure the parent keeps processing its subgoal list
		//we must return the 'active' status.
		if (subCommandState == completed && subCommandList.size() > 1)
		{
			return active;
		}

		return subCommandState;
	}
	//no more subgoals to process - return 'completed'
	else
	{
		return completed;
	}
}

void CompoundCharacterCommand :: AddSubCommand(CharacterCommand* c)
{   
	subCommandList.push_back(c);
}

void CompoundCharacterCommand :: RemoveAllSubCommands()
{
	for(std::list<CharacterCommand*>::iterator it = subCommandList.begin(); it != subCommandList.end(); ++it)
	{  
		(*it)->Terminate();
		delete *it;
	}

	subCommandList.clear();
} 

bool CompoundCharacterCommand :: IsCurrentCommand(CharacterCommand* c)
{
	if(subCommandList.size() > 0 && subCommandList.front()->GetType() == c->GetType())
	{
		return true;
	}

	return false;
}

bool CompoundCharacterCommand :: IsCurrentCommand(CommandType type)
{
	if(!subCommandList.empty() && subCommandList.front()->GetType() == type)
	{
		return true;
	}

	return false;
}
