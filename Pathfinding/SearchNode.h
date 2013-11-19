#ifndef SEARCH_NODE
#define SEARCH_NODE

#include <math.h>

struct SearchNode
{
	SearchNode(Node* setNode)
	{
		node = setNode;
	}

	Node*					node;	// the navgraph node represented by this search node
	int						g;		// cost so far
	int						h;		// estimated cost
	int						f;
	SearchNode*				parent;
};

#endif