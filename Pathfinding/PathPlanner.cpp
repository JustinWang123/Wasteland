#include "PathPlanner.h"
#include "GameManager.h"
#include "AStarPathToPositionGoal.h"
#include "AStarPathToCoverGoal.h"
#include <string>

PathPlanner* PathPlanner :: Instance()
{
	static PathPlanner instance;
	return &instance;
}

void PathPlanner :: SetNavGraph(NavigationGraph* setNavGraph)
{
	navGraph = setNavGraph;
}

int PathPlanner :: GetPathToPos(vector3df startPos, vector3df endPos, std::vector<vector3df> *path)
{
	// Make sure the path is empty:
	path->clear();

	// The Path vector is aranged so that the exact endPos is the first element and
	// the startPos is the last element.  Characters using the path will pop
	// elements from the back of the vector so the final element popped will be the exact
	// endPos.  We push it onto the path now so that it will be the first element.
	path->push_back(endPos);

	AStarPathToPositionGoal* goal = new AStarPathToPositionGoal(endPos, navGraph);
	
	int pathLength = CalculatePath(startPos, path, goal);
	delete goal;

	return pathLength;
}

int	PathPlanner :: GetPathToCover(vector3df startPos, vector3df viewerPos, std::vector<vector3df> *path)
{
	// Make sure the path is empty:
	path->clear();

	AStarPathToCoverGoal* goal = new AStarPathToCoverGoal(viewerPos, navGraph);
	int pathLength = CalculatePath(startPos, path, goal);
	delete goal;

	return pathLength;
}

int PathPlanner :: CalculatePath(vector3df startPos, std::vector<vector3df> *path, AStarGoalBase* goal)
{
	// For debugging first set all the nodes to white:
	navGraph->ResetNavGraphVisuals();

	int exitState = 0;

	// Get the graph node that is nearest the startPos:
	Node* startNode = navGraph->GetNearestNode(startPos);

	// Use the first graph node to create the first searchNode
	SearchNode* startSearchNode = new SearchNode(startNode);
	startSearchNode->parent = nullptr;
	startSearchNode->g = 0;
	startSearchNode->h = goal->CalculateH(startSearchNode->node);
	startSearchNode->f = startSearchNode->g + startSearchNode->h;

	// Push the first searchNode onto the open list to behind search:
	openList.push_back(startSearchNode);

	// Each time through the loops incriment this variable
	// We can then halt the search early if the loopCount exceeds the maxLoops
	int loopCount = 0;

	while(true)
	{
		// First Halt Condition:
		if(openList.empty())
		{
			exitState = -1;
			break;
		}
		
		// Second Halt Condition:
		if(loopCount > goal->GetMaxLoops())
		{
			exitState = -2;
			break;
		}

		// Remove the SearchNode with the lowest F cost from the open list:
		SearchNode* currentNode = PopOpenListF();
		
		// Third Halt Condition:
		if(goal->IsGoalNode(currentNode->node))
		{
			// Return path:
			exitState = CreateVectorPath(currentNode, path);
			break;
		}

		for(int i = 0; i < currentNode->node->adjacentNodeList.size(); i++)
		{
			AddChild(currentNode, currentNode->node->adjacentNodeList[i], goal->CalculateH(currentNode->node->adjacentNodeList[i]), currentNode->node->adjacentCostList[i]);
		}

		closedList.push_back(currentNode);
		loopCount++;
	}

	// Delete all search nodes in the open and closed list:
	for (std::vector<SearchNode*>::iterator it = openList.begin(); it != openList.end(); it++)		delete (*it);
	openList.clear();
	for (std::vector<SearchNode*>::iterator it = closedList.begin(); it != closedList.end(); it++)	delete (*it);
	closedList.clear();

	return exitState;
}

void PathPlanner :: AddChild(SearchNode* parent, Node* child, int hCost, int gCost)
{
	if( !IsOnClosedList(child) )
		if(!IsOnOpenList(child) )
		{
			// Node is not on any lists so create it:
			SearchNode* childSearchNode = new SearchNode(child);
			childSearchNode->parent = parent;
			childSearchNode->g = parent->g + gCost;
			childSearchNode->h = hCost;
			childSearchNode->f = childSearchNode->g + childSearchNode->h;
			openList.push_back( childSearchNode );
		}
		else 
		{
			// If child node is already on the open list then get the search node referencing it:
			SearchNode* temp = GetNodeFromOpenList( child );

			// If path from start to child through parent is shorter then the path to child already on the open list:
			// Change the parent of the node already on the list to be this parent:
			if(parent->g + gCost < temp->g)
			{
				temp->parent = parent;
				temp->g = parent->g + gCost;
				temp->f = temp->h + temp->g;
			}
		}
}

int PathPlanner :: CreateVectorPath(SearchNode* goalNode, std::vector<vector3df> *path)
{
	// Start at the goalNode and work backwards:
	SearchNode* currentNode = goalNode;
	int pathLength = 0;

	// Add positions to the back of the path until we reach the start pos
	while(currentNode->parent != nullptr)
	{
		pathLength++;
		currentNode->node->sceneNode->setColor(SColor(255,0,0,255));
		path->push_back(currentNode->node->position);
		currentNode = currentNode->parent;
	}

	return pathLength;
}

bool PathPlanner :: IsOnOpenList(Node* node)
{
	for (int i = 0; i < openList.size(); i++)
		if(openList[i]->node == node)
		{
			return true;
		}

	return false;
}

bool PathPlanner :: IsOnClosedList(Node* node)
{
	for (int i = 0; i < closedList.size(); i++)
		if(closedList[i]->node == node)
		{
			return true;
		}

	return false;
}

SearchNode* PathPlanner :: GetNodeFromOpenList(Node* node)
{
		for (int i = 0; i < openList.size(); i++)
		if(openList[i]->node == node)
		{
			return openList[i];
		}

	return NULL;
}

SearchNode* PathPlanner :: PopOpenListF()
{
	int lowestValue = 100000;
	SearchNode* lowestNode = NULL;
	int lowestNodeID = -1;

	for (int i = 0; i < openList.size(); i++)
	{

		if(openList[i]->f < lowestValue)
		{
			lowestValue = openList[i]->f;
			lowestNode = openList[i];
			lowestNodeID = i;
		}
	}

	openList.erase(openList.begin() + lowestNodeID);

	return lowestNode;
}

