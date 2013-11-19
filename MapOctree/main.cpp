#include "MapOctree.h"
#include <iostream>

int main()
{
	MapOctree mapOctree(0, 0, 0, 4, 4, 4, 0);

	for(int x = 0; x < 4; x++)
		for(int y = 0; y < 4; y++)
			for(int z = 0; z < 4; z++){
				mapOctree.SetValue(x,y,z,1);
				std::cout << "Num of MapOctrees(1): " << MapOctree::numOfMapOctrees << std::endl;

			}




	return 0;
}

