#ifndef ASTAR_PATH_TO_COVER_GOAL
#define ASTAR_PATH_TO_COVER_GOAL

#include "AStarGoalBase.h"
#include "IrrWrapper.h"
#include "NavigationGraph.h"
#include "GameManager.h"


class   AStarPathToCoverGoal : public AStarGoalBase
{
public:
	AStarPathToCoverGoal(CharacterInstance* setOwner, NavigationGraph *navGraph)
		: AStarGoalBase(1000)
	{
		owner = setOwner;
	}

	~AStarPathToCoverGoal(){}

	virtual int CalculateH(NavNode* node);
	virtual bool IsGoalNode(NavNode* node);

private:
	CharacterInstance* owner;
};


inline int AStarPathToCoverGoal :: CalculateH(NavNode* node)
{
	return 0;
}

inline bool AStarPathToCoverGoal ::IsGoalNode(NavNode* node)
{
	//	We define a 'cover position' as a position which is adjacent to a half wall
	//	Also it must block line of sight from the player when he is ducked
	//	Also it must allow line of sight when the player is standing
	if(!node->isOccupied && GManager->IsCharCoveredFromAllHostilesAtPos(owner, node->position))
		return true;
	else
		return false;

}

#endif