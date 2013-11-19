#ifndef ASTAR_COMMAND_PATH_TO_POSITION_GOAL
#define ASTAR_COMMAND_PATH_TO_POSITION_GOAL

#include "AStarGoalBase.h"
#include "IrrWrapper.h"


class   AStarGoalPathToPosition : public AStarGoalBase
{
public:
	AStarGoalPathToPosition(vector3df setDestination);
	~AStarGoalPathToPosition(){}

	virtual int CalculateH(vector3di position);
	virtual bool IsGoalNode(vector3di position);

private:
	vector3df destinationPos;
};


#endif
