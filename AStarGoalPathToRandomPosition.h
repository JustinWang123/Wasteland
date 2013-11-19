#ifndef ASTAR_GOAL_PATH_TO_RANDOM_POSITION_H
#define ASTAR_GOAL_PATH_TO_RANDOM_POSITION_H

#include "AStarGoalBase.h"
#include "IrrWrapper.h"

class   AStarGoalPathToRandomPosition : public AStarGoalBase
{
public:
	AStarGoalPathToRandomPosition(vector3df setStartPos, int setMaxDistance);
	~AStarGoalPathToRandomPosition(){}

	virtual int CalculateH(vector3di position);
	virtual bool IsGoalNode(vector3di position);

private:
	vector3df startPos;
	int maxDistance;
};


#endif