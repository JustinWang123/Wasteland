#include "NavigationGraph.h"
#include "LevelInstance.h"
#include "IrrWrapper.h"
	
// Takes in a 2D array indicating which tiles are open (walkable)
// Generates the navgraph from this data
void NavigationGraph :: CreateNavGraph()
{
	Node* nodeMapList[MAP_SIZE_X][MAP_SIZE_Z];

	float nodeYPos = 0.1f;

	// Add all open nodes to the nodeMapList:
	for(int x = 0; x < MAP_SIZE_X; x++)
		for(int z = 0; z < MAP_SIZE_Z; z++)
			if(!owner->CheckCollision(vector3df(x*2 + 1, 0, z*2 + 1)) || owner->IsTileCover(x,z))
			{
				nodeMapList[x][z] = new Node(vector3df(x*2 + 1, 0 ,z*2 + 1));
				nodeMapList[x][z]->sceneNode = IRR::scene_manager->addBillboardSceneNode(0, dimension2d<f32>(0.2,0.2), vector3df(x*2 + 1, nodeYPos, z*2 + 1), 0);	
				nodeMapList[x][z]->sceneNode->setMaterialFlag(EMF_LIGHTING, false);
				nodeMapList[x][z]->sceneNode->setVisible(true);
			}
			else
			{
				nodeMapList[x][z] = NULL;
			}

	// Connect all nodes on nodeMapList:
	for(int x = 0; x < MAP_SIZE_X; x++)
		for(int z = 0; z < MAP_SIZE_Z; z++)
			if(nodeMapList[x][z] != NULL)
			{
				bool isCover = owner->IsTileCover(x,z);

				// Horizontal and Vertical:
				if(x >= 0 && x < MAP_SIZE_X
				&& z - 1 >= 0 && z - 1 < MAP_SIZE_Z
				&& nodeMapList[x][z - 1]
				&& !owner->IsTileCover(x,z - 1))
				{
					if(isCover) // If the node is a cover node we let all the adjacent nodes know
					{
						nodeMapList[x][z - 1]->isAdjacentToCover = true;
					}
					else
					{
						nodeMapList[x][z]->adjacentNodeList.push_back(nodeMapList[x][z - 1]);
						nodeMapList[x][z]->adjacentCostList.push_back(10);
					}
				}

				if(x >= 0 && x < MAP_SIZE_X
				&& z + 1 >= 0 && z + 1 < MAP_SIZE_Z
				&& nodeMapList[x][z + 1]
				&& !owner->IsTileCover(x,z + 1))
				{
					if(isCover)
					{
						nodeMapList[x][z + 1]->isAdjacentToCover = true;
					}
					else
					{
						nodeMapList[x][z]->adjacentNodeList.push_back(nodeMapList[x][z + 1]);
						nodeMapList[x][z]->adjacentCostList.push_back(10);
					}
				}

				if(x - 1 >= 0 && x - 1 < MAP_SIZE_X
				&& z >= 0 && z < MAP_SIZE_Z
				&& nodeMapList[x - 1][z]
				&& !owner->IsTileCover(x - 1,z))
				{
					if(isCover)
					{
						nodeMapList[x - 1][z]->isAdjacentToCover = true;
					}
					else
					{
						nodeMapList[x][z]->adjacentNodeList.push_back(nodeMapList[x - 1][z]);
						nodeMapList[x][z]->adjacentCostList.push_back(10);
					}
				}

				if(x + 1 >= 0 && x + 1 < MAP_SIZE_X
				&& z >= 0 && z < MAP_SIZE_Z
				&& nodeMapList[x + 1][z]
				&& !owner->IsTileCover(x + 1,z))
				{
					if(isCover)
					{
						nodeMapList[x + 1][z]->isAdjacentToCover = true;
					}
					else
					{
						nodeMapList[x][z]->adjacentNodeList.push_back(nodeMapList[x + 1][z]);
						nodeMapList[x][z]->adjacentCostList.push_back(10);
					}
				}

				// Diagonals:
				if(x + 1 >= 0 && x + 1 < MAP_SIZE_X
				&& z - 1 >= 0 && z - 1 < MAP_SIZE_Z
				&& nodeMapList[x + 1][z - 1]
				&& ((nodeMapList[x][z - 1] && !owner->IsTileCover(x,z-1) ) || (nodeMapList[x + 1][z]) && !owner->IsTileCover(x + 1,z))
				&& !owner->IsTileCover(x + 1,z - 1))
				{
					if(isCover)
					{
						nodeMapList[x + 1][z - 1]->isAdjacentToCover = true;
					}
					else
					{
						nodeMapList[x][z]->adjacentNodeList.push_back(nodeMapList[x + 1][z - 1]);
						nodeMapList[x][z]->adjacentCostList.push_back(14);
					}
				}

				if(x - 1 >= 0 && x - 1 < MAP_SIZE_X
				&& z + 1 >= 0 && z + 1 < MAP_SIZE_Z
				&& nodeMapList[x - 1][z + 1]
				&& ((nodeMapList[x][z + 1] && !owner->IsTileCover(x ,z + 1)) || (nodeMapList[x - 1][z] && !owner->IsTileCover(x - 1,z)))
				&& !owner->IsTileCover(x - 1,z + 1))
				{
					if(isCover)
					{
						nodeMapList[x - 1][z + 1]->isAdjacentToCover = true;
					}
					else
					{
						nodeMapList[x][z]->adjacentNodeList.push_back(nodeMapList[x - 1][z + 1]);
						nodeMapList[x][z]->adjacentCostList.push_back(14);
					}

					
				}

				if(x - 1 >= 0 && x - 1 < MAP_SIZE_X
				&& z - 1 >= 0 && z - 1 < MAP_SIZE_Z
				&& nodeMapList[x - 1][z - 1]
				&& ((nodeMapList[x][z - 1] && !owner->IsTileCover(x ,z - 1)) || (nodeMapList[x - 1][z] && !owner->IsTileCover(x - 1,z)))
				&& !owner->IsTileCover(x - 1,z - 1))
				{
					if(isCover)
					{
						nodeMapList[x - 1][z - 1]->isAdjacentToCover = true;
					}
					else
					{
						nodeMapList[x][z]->adjacentNodeList.push_back(nodeMapList[x - 1][z - 1]);
						nodeMapList[x][z]->adjacentCostList.push_back(14);
					}
				}

				if(x + 1 >= 0 && x + 1 < MAP_SIZE_X
				&& z + 1 >= 0 && z + 1 < MAP_SIZE_Z
				&& nodeMapList[x + 1][z + 1]
				&& ((nodeMapList[x + 1][z] && !owner->IsTileCover(x + 1,z))|| (nodeMapList[x][z + 1] && !owner->IsTileCover(x,z + 1)))
				&& !owner->IsTileCover(x + 1,z + 1))
				{
					if(isCover)
					{
						nodeMapList[x + 1][z + 1]->isAdjacentToCover = true;
					}
					else
					{
						nodeMapList[x][z]->adjacentNodeList.push_back(nodeMapList[x + 1][z + 1]);
						nodeMapList[x][z]->adjacentCostList.push_back(14);
					}
				}

				// If the node is cover then we can actually delete it
				// It has only been kept so far so as to assign all adjacent nodes 'adjacentToCover' status
				if(isCover)
				{
					nodeMapList[x][z]->sceneNode->remove();
					delete nodeMapList[x][z];
					nodeMapList[x][z] = nullptr;
				}
			}


	for(int x = 0; x < MAP_SIZE_X; x++)
		for(int z = 0; z < MAP_SIZE_Z; z++)
			if(nodeMapList[x][z])
			{
				nodeVector.push_back(nodeMapList[x][z]);
			}

}

Node* NavigationGraph :: GetNearestNode(vector3df position)
{
	float leastDistanceSQ = 500000;
	Node* currentNode = NULL;

	for (std::vector<Node*>::iterator it = nodeVector.begin(); it != nodeVector.end(); it++)
	{
		float tempDistance = position.getDistanceFromSQ((*it)->position);

		if(tempDistance < leastDistanceSQ)
		{
			currentNode = (*it);
			leastDistanceSQ = tempDistance;
		}
	}

	return currentNode;
}

void NavigationGraph :: ResetNavGraphVisuals()
{
	for (std::vector<Node*>::iterator it = nodeVector.begin(); it != nodeVector.end(); it++)
	{
		if( (*it)->isAdjacentToCover )
		{
			(*it)->sceneNode->setColor(SColor(255,255,0,0));
		}
		else
		{
			(*it)->sceneNode->setColor(SColor(255,255,255,255));
		}
	}
}