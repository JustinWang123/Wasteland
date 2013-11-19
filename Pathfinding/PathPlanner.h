#ifndef PATH_PLANNER
#define PATH_PLANNER

#include "IrrWrapper.h"
#include "NavigationGraph.h"
#include "SearchNode.h"
#include "Node.h"
#include <vector>
#include "AStarGoalBase.h"

class CharacterInstance;

/*
	PathPlanner is a globally accessible singleton class which will create paths to
	various different goals for the characters.
*/
class PathPlanner
{
public:
	static PathPlanner* Instance();
	void	SetNavGraph(NavigationGraph* setNavGraph);
	
	int		GetPathToPos		(vector3df startPos, vector3df endPos, std::vector<vector3df> *path);
	int		GetPathToCover	(vector3df startPos, vector3df viewerPos, std::vector<vector3df> *path);

private:
	PathPlanner(){}

	//	Searches the Navgraph for a node which satisfies the condition in GoalTest()
	//	Dirrects its search based on the value returned by CalculateH()
	int		CalculatePath(vector3df startPos, std::vector<vector3df> *path, AStarGoalBase* goal);

	//	A searchNode will use this method to add its children to the openList
	//	If the child is no present on the open list the simply add it to open list
	//	If the child is already present on the open list and the path through parent is a more efficient route
	//	then the node already on the open list has its parent set to parent
	void	AddChild(SearchNode* parent, Node* child, int hCost, int gCost);

	// Starting from the goalNode, works backwards adding the position of each node to the
	// path vector until it encounters a SearchNode with no parent (the starting node)
	// Path will contain all the positions that make up the path with the startPos at the back
	// Returns the length of the path
	int CreateVectorPath(SearchNode* goalNode, std::vector<vector3df> *path);

	bool						IsOnOpenList(Node* node);
	bool						IsOnClosedList(Node* node);
	SearchNode*					GetNodeFromOpenList(Node* node);
	SearchNode*					PopOpenListF();

	NavigationGraph*			navGraph;
	std::vector<SearchNode*>	openList;
	std::vector<SearchNode*>	closedList;
};

#define PPlanner PathPlanner::Instance()

#endif