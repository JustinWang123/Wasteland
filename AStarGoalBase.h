#ifndef ASTAR_GOAL_BASE
#define ASTAR_GOAL_BASE

#include "IrrWrapper.h"

const int MAX_LOOPS = 10000;


class   AStarGoalBase
{
public:
	AStarGoalBase(int setMaxLoops){maxLoops = setMaxLoops;}
	~AStarGoalBase(){}

	// Estimates the distance from the argument node to whatever the goal is:
	virtual int CalculateH(vector3di pos) = 0; 

	virtual bool IsGoalNode(vector3di pos) = 0;

	int GetMaxLoops() const {return maxLoops;}

protected:
	// Each AStarGoal will have a different maximum number of loops to run:
	int maxLoops;
};
#endif