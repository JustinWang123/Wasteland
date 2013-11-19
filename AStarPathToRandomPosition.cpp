#include "AStarGoalPathToRandomPosition.h"

AStarGoalPathToRandomPosition :: AStarGoalPathToRandomPosition(vector3df setStartPos, int setMaxDistance)
	: AStarGoalBase(MAX_LOOPS)
{
	startPos = setStartPos;
	maxDistance = setMaxDistance;
}



int AStarGoalPathToRandomPosition :: CalculateH(vector3di position)
{
	vector3df pos(position.X + 0.5f, position.Y + 0.5f, position.Z + 0.5f);

	// Negative so fartherest positions are considered 'better'
	return 0;//-(abs((int)(pos.X - startPos.X)) + abs((int)(position.Y - startPos.Y)) + abs((int)(pos.Z - startPos.Z))) * 10; 
}

 bool AStarGoalPathToRandomPosition :: IsGoalNode(vector3di position)
{
	vector3df pos(position.X, position.Y, position.Z);

	float distance = pos.getDistanceFrom(startPos);

	if( rand()%maxDistance == 0 || distance > maxDistance)
	{
		return true;
	}
	else
	{
		return false;
	}
}
