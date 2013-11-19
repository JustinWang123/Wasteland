#ifndef NODE
#define NODE

#include "IrrWrapper.h"
#include <vector>

// Each node is responsible for maintaining a list of pointers indicating adjacent nodes
// The navigation graph can be queried by the path planner to obtain a route between nodes
struct Node
{
	Node(vector3df setPos): position(setPos)
	{
		isAdjacentToCover = false;
	}
	vector3df position;
	bool isAdjacentToCover;
	std::vector<Node*> adjacentNodeList;
	std::vector<int> adjacentCostList;
	IBillboardSceneNode* sceneNode; // Use for debugging to visualize nodes
};

#endif