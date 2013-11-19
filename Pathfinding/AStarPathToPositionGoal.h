#ifndef ASTAR_PATH_TO_POSITION_GOAL
#define ASTAR_PATH_TO_POSITION_GOAL

#include "AStarGoalBase.h"
#include "IrrWrapper.h"
#include "NavigationGraph.h"

const int MAX_LOOPS = 10000;

class AStarPathToPositionGoal : public AStarGoalBase
{
public:
	AStarPathToPositionGoal(vector3df setDestination, NavigationGraph *navGraph)
		: AStarGoalBase(MAX_LOOPS)
	{
		destinationNode = navGraph->GetNearestNode(setDestination);
	}

	~AStarPathToPositionGoal(){}

	virtual int CalculateH(Node* node);
	virtual bool IsGoalNode(Node* node);

private:
	Node* destinationNode;
};

// Use Manhattan Distance as the heuristic for the PathToPosition Goal:
inline int AStarPathToPositionGoal :: CalculateH(Node* node)
{
	return abs((int)(node->position.X - destinationNode->position.X)) + abs((int)(node->position.Z - destinationNode->position.Z));
}

inline bool AStarPathToPositionGoal ::IsGoalNode(Node* node)
{
	if(node == destinationNode)
		return true;
	else
		return false;

}

#endif