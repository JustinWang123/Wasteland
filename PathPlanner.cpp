#include "PathPlanner.h"
#include "GameManager.h"
#include <string>
#include <iostream>
#include "AStarGoalPathToPosition.h"
#include "AStarGoalPathToRandomPosition.h"

PathPlanner* PathPlanner :: Instance()
{
	static PathPlanner instance;
	return &instance;
}

void PathPlanner :: Init()
{
	maxLoops = 100;
}

int PathPlanner :: GetPathToPos(vector3df startPos, vector3df endPos, std::list<vector3df> *path)
{
	// Make sure the path is empty:
	path->clear();

	AStarGoalPathToPosition goal(endPos);

	int pathLength = CalculatePath(vector3di(startPos.X, startPos.Y, startPos.Z), path, &goal);

	return pathLength;
}

int PathPlanner :: GetPathToRandomPos(vector3df startPosF, int maxDistance, std::list<vector3df> *path)
{
	// Make sure the path is empty:
	path->clear();

	vector3di startPos((int)startPosF.X, (int)startPosF.Y, (int)startPosF.Z);


	// Set dirrection to random:
	vector3di dirrection;
	dirrection.X = rand()%3 - 1;
	dirrection.Y = 0;
	dirrection.Z = rand()%3 - 1;

	for(int i = 0; i < maxDistance; i++)
		if(MapMgr->IsVoxelWalkable(startPos + dirrection * i, 2))
		{
			path->push_front(vector3df(startPos.X + dirrection.X * i + 0.5f, startPos.Y + dirrection.Y * i + 0.1f, startPos.Z + dirrection.Z * i + 0.5f) );
		}
		else
		{
			break;
		}

	std::cout << "Found random path of length: " << path->size() << std::endl;

	return path->size();
}

int PathPlanner :: CalculatePath(vector3di startVoxel, std::list<vector3df> *path, AStarGoalBase* goal)
{
	int exitState = 0;

	// Use the first graph node to create the first searchNode
	SearchNode* startSearchNode = new SearchNode(startVoxel);
	startSearchNode->parentSearchNode = 0;
	startSearchNode->g = 0;
	startSearchNode->h = goal->CalculateH(startSearchNode->voxel);
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
		if(loopCount > maxLoops)
		{
			exitState = -2;
			break;
		}

		// Remove the SearchNode with the lowest F cost from the open list:
		SearchNode* currentNode = PopOpenListF();

		// Third Halt Condition:
		if(goal->IsGoalNode(currentNode->voxel))
		{
			// Return path:
			exitState = CreateVectorPath(currentNode, path);
			break;
		}

		for(int x = -1; x <= 1; x++)
			for(int z = -1; z <= 1; z++)
				if( !(x == 0 && z == 0) )
				{
					int moveCost = 10;

					if((abs(x) + abs(z)) == 2)
						moveCost = 14;

					if(MapMgr->IsVoxelWalkable( currentNode->voxel + vector3di(x,0,z), 2))
					{
						AddChild(currentNode, currentNode->voxel + vector3di(x,0,z), goal->CalculateH(currentNode->voxel + vector3di(x, 0, z)), moveCost);
					}
					else if(MapMgr->IsVoxelWalkable( currentNode->voxel + vector3di(x,1,z), 2))
					{
						AddChild(currentNode, currentNode->voxel + vector3di(x,1,z), goal->CalculateH(currentNode->voxel + vector3di(x,1,z)), moveCost);
					}
					else if(MapMgr->IsVoxelWalkable( currentNode->voxel + vector3di(x,-1,z), 2))
					{
						AddChild(currentNode, currentNode->voxel + vector3di(x,-1,z), goal->CalculateH(currentNode->voxel + vector3di(x,-1,z)), moveCost);
					}


				}

		closedList.push_back(currentNode);
		loopCount++;
	}

	if(exitState == -1)
	{
		std::cout << "No path exists!!!" << std::endl;
	}
	else if(exitState == -2)
	{
		std::cout << "Path finding exceeded " << maxLoops << " iterations" << std::endl;
	}
	else
	{
		std::cout << "Path length: " << path->size() << std::endl;
		std::cout << "Path finding took: " << loopCount << " iterations" << std::endl;
	}

	// Delete all search nodes in the open and closed list:
	for (std::vector<SearchNode*>::iterator it = openList.begin(); it != openList.end(); it++)
		delete (*it);
	openList.clear();

	for (std::vector<SearchNode*>::iterator it = closedList.begin(); it != closedList.end(); it++)
		delete (*it);
	closedList.clear();

	return exitState;
}

void PathPlanner :: AddChild(SearchNode* parent, vector3di childVoxel, int hCost, int gCost)
{
	if( !IsOnClosedList(childVoxel) )
	{
		if(!IsOnOpenList(childVoxel) )
		{
			// Node is not on any lists so create it:
			SearchNode* childSearchNode = new SearchNode(childVoxel);
			childSearchNode->parentSearchNode = parent;
			childSearchNode->g = parent->g + gCost;
			childSearchNode->h = hCost;
			childSearchNode->f = childSearchNode->g + childSearchNode->h;
			openList.push_back( childSearchNode );
		}
		else
		{
			// If child node is already on the open list then get the search node referencing it:
			SearchNode* temp = GetNodeFromOpenList( childVoxel );

			// If path from start to child through parent is shorter then the path to child already on the open list:
			// Change the parent of the node already on the list to be this parent:
			if(parent->g + gCost < temp->g)
			{
				temp->parentSearchNode = parent;
				temp->g = parent->g + gCost;
				temp->f = temp->h + temp->g;
			}
		}
	}
}

int PathPlanner :: CreateVectorPath(SearchNode* goalNode, std::list<vector3df> *path)
{
	// Start at the goalNode and work backwards:
	SearchNode* currentNode = goalNode;
	int pathLength = 0;

	// Add positions to the back of the path until we reach the start pos
	while(currentNode->parentSearchNode != 0)
	{
		pathLength++;

		// Create positions using centers of each voxel:
		// Note we dont add 0.5 to y cause player walks on the bottem of voxel
		path->push_back( vector3df(currentNode->voxel.X + 0.5f, currentNode->voxel.Y + 0.1f, currentNode->voxel.Z + 0.5f) );

		currentNode = currentNode->parentSearchNode;
	}

	return pathLength;
}

bool PathPlanner :: IsOnOpenList(vector3di voxel)
{
	for (unsigned int i = 0; i < openList.size(); i++)
		if(openList[i]->voxel == voxel)
		{
			return true;
		}

	return false;
}

bool PathPlanner :: IsOnClosedList(vector3di voxel)
{
	for (unsigned int i = 0; i < closedList.size(); i++)
		if(closedList[i]->voxel == voxel)
		{
			return true;
		}

	return false;
}

SearchNode* PathPlanner :: GetNodeFromOpenList(vector3di voxel)
{
		for (unsigned int i = 0; i < openList.size(); i++)
		if(openList[i]->voxel == voxel)
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

	for (unsigned int i = 0; i < openList.size(); i++)
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

int PathPlanner :: CalculateH(vector3di position, vector3di destination)
{
	return (abs((int)position.X - (int)destination.X)
			+ abs((int)position.Y - (int)destination.Y)
			+ abs((int)position.Z - (int)destination.Z)) * 10;
}
