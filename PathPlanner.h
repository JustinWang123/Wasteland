#ifndef PATH_PLANNER
#define PATH_PLANNER

#include "IrrWrapper.h"
#include "AStarGoalBase.h"
#include <list>
#include <vector>

struct SearchNode
{
	SearchNode(vector3di setVoxel) 
		: voxel(setVoxel){}

	vector3di				voxel;		// the voxel coordinate represented by this search node
	int						g;			// cost so far
	int						h;			// estimated cost
	int						f;
	SearchNode*				parentSearchNode;
};

class CharacterInstance;

/*
	PathPlanner is a globally accessible singleton class which will create paths to
	various different goals for the characters.
*/
class PathPlanner
{
public:
	static PathPlanner* Instance();

	void	Init();

	int		GetPathToPos(vector3df startPos, vector3df endPos, std::list<vector3df> *path);
	int		GetPathToRandomPos(vector3df startPos, int maxDistance, std::list<vector3df> *path);

	int		CalculateH(vector3di position, vector3di destination);

private:
	PathPlanner(){}

	//	Searches the Navgraph for a node which satisfies the condition in GoalTest()
	//	Dirrects its search based on the value returned by CalculateH()
	int		CalculatePath(vector3di startVoxel, std::list<vector3df> *path, AStarGoalBase* goal);

	//	A searchNode will use this method to add its children to the openList
	//	If the child is no present on the open list the simply add it to open list
	//	If the child is already present on the open list and the path through parent is a more efficient route
	//	then the node already on the open list has its parent set to parent
	void	AddChild(SearchNode* parent, vector3di child, int hCost, int gCost);

	// Starting from the goalNode, works backwards adding the position of each node to the
	// path vector until it encounters a SearchNode with no parent (the starting node)
	// Path will contain all the positions that make up the path with the startPos at the back
	// Returns the length of the path
	int CreateVectorPath(SearchNode* goalNode, std::list<vector3df> *path);

	
	bool						IsOnOpenList(vector3di voxel);
	bool						IsOnClosedList(vector3di voxel);
	SearchNode*					GetNodeFromOpenList(vector3di voxel);
	SearchNode*					PopOpenListF();

	std::vector<SearchNode*>	openList;
	std::vector<SearchNode*>	closedList;

	int maxLoops;
};

#define PPlanner PathPlanner::Instance()

#endif