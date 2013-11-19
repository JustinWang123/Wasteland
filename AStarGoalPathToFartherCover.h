#ifndef ASTAR_PATH_TO_FARTHER_COVER_GOAL
#define ASTAR_PATH_TO_FARTHER_COVER_GOAL

#include "AStarGoalBase.h"
#include "IrrWrapper.h"
#include "NavigationGraph.h"
#include "GameManager.h"


class   AStarPathToFartherCoverGoal : public AStarGoalBase
{
public:
	AStarPathToFartherCoverGoal(vector3df setCurrentPos, vector3df setViewerPos, NavigationGraph *navGraph)
		: AStarGoalBase(500)
	{
		viewerPos = setViewerPos;
		vector3df currentNodePos = ((NavNode*)navGraph->GetNearestNode(setCurrentPos))->position;
		currentDisSQ = currentNodePos.getDistanceFromSQ(viewerPos);
	}

	~AStarPathToFartherCoverGoal(){}

	virtual int CalculateH(NavNode* node);
	virtual bool IsGoalNode(NavNode* node);

private:
	vector3df viewerPos;
	float currentDisSQ;
};


inline int AStarPathToFartherCoverGoal :: CalculateH(NavNode* node)
{
	// Negative to favor nodes 'farther' from the viewer
	return -(abs((int)(node->position.X - viewerPos.X)) + abs((int)(node->position.Z - viewerPos.Z)));

}

inline bool AStarPathToFartherCoverGoal ::IsGoalNode(NavNode* node)
{
	//	Before we check anything see if the position is even closer then our current
	//	We define a 'cover position' as a position which is adjacent to a half wall
	//	Also it must block line of sight from the player when he is ducked
	//	Also it must allow line of sight when the player is standing
	// 
	if(node->position.getDistanceFromSQ(viewerPos) > currentDisSQ
	&& !node->isOccupied && GManager->IsCoverBetween(node->position, viewerPos))
		return true;
	else
		return false;

}

#endif