#include "AStarGoalPathToPosition.h"

AStarGoalPathToPosition :: AStarGoalPathToPosition(vector3df setDestination)
	: AStarGoalBase(MAX_LOOPS)
{
	destinationPos = setDestination;
}


// Use Manhattan Distance as the heuristic for the PathToPosition Goal:
int AStarGoalPathToPosition :: CalculateH(vector3di position)
{
	vector3df pos(position.X + 0.5f, position.Y + 0.5f, position.Z + 0.5f);

	return (abs((int)(pos.X - destinationPos.X)) + abs((int)(position.Y - destinationPos.Y)) + abs((int)(pos.Z - destinationPos.Z))) * 10; 
}

 bool AStarGoalPathToPosition :: IsGoalNode(vector3di position)
{
	vector3di dest(destinationPos.X, destinationPos.Y, destinationPos.Z);

	if(dest == position)
		return true;
	else
		return false;
}
