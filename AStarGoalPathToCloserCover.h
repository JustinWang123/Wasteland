#ifndef ASTAR_PATH_TO_CLOSER_COVER_GOAL
#define ASTAR_PATH_TO_CLOSER_COVER_GOAL

#include "AStarGoalBase.h"
#include "IrrWrapper.h"
#include "NavigationGraph.h"
#include "GameManager.h"
#include "CharacterInstance.h"


class   AStarPathToCloserCoverGoal : public AStarGoalBase
{
public:
	AStarPathToCloserCoverGoal(CharacterInstance* setOwner, vector3df setDesiredPos, NavigationGraph *navGraph)
		: AStarGoalBase(500)
	{
		owner = setOwner;
		desiredPos = setDesiredPos;
		vector3df currentNodePos = ((NavNode*)navGraph->GetNearestNode(owner->getPosition()))->position;
		currentDisSQ = currentNodePos.getDistanceFromSQ(setDesiredPos);
	}

	~AStarPathToCloserCoverGoal(){}

	virtual int CalculateH(NavNode* node);
	virtual bool IsGoalNode(NavNode* node);

private:
	CharacterInstance* owner;
	vector3df desiredPos;
	float currentDisSQ;
};


inline int AStarPathToCloserCoverGoal :: CalculateH(NavNode* node)
{
	return abs((int)(node->position.X - desiredPos.X)) + abs((int)(node->position.Z - desiredPos.Z));
	//return 0;
}

inline bool AStarPathToCloserCoverGoal ::IsGoalNode(NavNode* node)
{
	//	Before we check anything see if the position is even closer then our current
	//	We define a 'cover position' as a position which is adjacent to a half wall
	//	Also it must block line of sight from the player when he is ducked
	//	Also it must allow line of sight when the player is standing
	// 
	if(node->position.getDistanceFromSQ(desiredPos) < currentDisSQ
	&& !node->isOccupied && GManager->IsCharCoveredFromAllHostilesAtPos(owner, node->position))
		return true;
	else
		return false;

}

#endif