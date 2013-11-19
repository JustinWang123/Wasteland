#ifndef NAVIGATION_GRAPH
#define NAVIGATION_GRAPH

#include "IrrWrapper.h"
#include "Node.h"
#include <vector>


class LevelInstance;




// This class holds a vector containing all navigation nodes on the map
class NavigationGraph
{
public:
	NavigationGraph(LevelInstance* setOwner) : owner(setOwner){}

	void CreateNavGraph();
	Node* GetNearestNode(vector3df position);
	void ResetNavGraphVisuals();

private:
	std::vector<Node*> nodeVector;
	LevelInstance* owner;
};

#endif