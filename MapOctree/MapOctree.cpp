#include "MapOctree.h"

int MapOctree :: numOfMapOctrees = 0;

MapOctree :: MapOctree(int setX0, int setY0, int setZ0, int setX1, int setY1, int setZ1, int setValue)
	: x0(setX0), y0(setY0), z0(setZ0), x1(setX1), y1(setY1), z1(setZ1), isLeaf(true), value(setValue) {
	MapOctree::numOfMapOctrees++;

	for(int xIt = 0; xIt < 2; xIt++)
		for(int yIt = 0; yIt < 2; yIt++)
			for(int zIt = 0; zIt < 2; zIt++)
				children[xIt][yIt][zIt] = 0;
}

MapOctree :: ~MapOctree() {
	MapOctree::numOfMapOctrees--;

	for(int xIt = 0; xIt < 2; xIt++)
		for(int yIt = 0; yIt < 2; yIt++)
			for(int zIt = 0; zIt < 2; zIt++)
				if(children[xIt][yIt][zIt] != 0) {
					delete children[xIt][yIt][zIt];
					children[xIt][yIt][zIt] = 0;
				}
}

void MapOctree :: SetValue(int x, int y, int z, int setVal) {
	if(isLeaf) {
		if(setVal == value || x1-x0 == 1){
			value = setVal;
		}
		else {
			isLeaf = false;

			children[0][0][0] = new MapOctree(x0, 				y0, 				z0, 				x0 + (x1 - x0)/2, 	y0 + (y1 - y0)/2, 	z0 + (z1 - z0)/2, 	value);
			children[0][0][1] = new MapOctree(x0, 				y0, 				z0 + (z1 - z0)/2, 	x0 + (x1 - x0)/2, 	y0 + (y1 - y0)/2, 	z1, 				value);
			children[0][1][0] = new MapOctree(x0, 				y0 + (y1 - y0)/2, 	z0, 				x0 + (x1 - x0)/2, 	y1, 				z0 + (z1 - z0)/2, 	value);
			children[0][1][1] = new MapOctree(x0, 				y0 + (y1 - y0)/2, 	z0 + (z1 - z0)/2, 	x0 + (x1 - x0)/2, 	y1, 				z1, 				value);
			children[1][0][0] = new MapOctree(x0 + (x1 - x0)/2, y0, 				z0, 				x1, 				y0 + (y1 - y0)/2, 	z0 + (z1 - z0)/2, 	value);
			children[1][0][1] = new MapOctree(x0 + (x1 - x0)/2, y0, 				z0 + (z1 - z0)/2, 	x1, 				y0 + (y1 - y0)/2, 	z1, 				value);
			children[1][1][0] = new MapOctree(x0 + (x1 - x0)/2, y0 + (y1 - y0)/2, 	z0, 				x1, 				y1, 				z0 + (z1 - z0)/2, 	value);
			children[1][1][1] = new MapOctree(x0 + (x1 - x0)/2, y0 + (y1 - y0)/2, 	z0 + (z1 - z0)/2, 	x1, 				y1, 				z1, 				value);

			children[GetQuadrantX(x)][GetQuadrantY(y)][GetQuadrantZ(z)]->SetValue(x, y, z, setVal);
		}
	}
	else {
		children[GetQuadrantX(x)][GetQuadrantY(y)][GetQuadrantZ(z)]->SetValue(x, y, z, setVal);

		bool collapseChildren = true;
		int sameValue = -1;

		for(int xIt = 0; xIt < 2; xIt++)
			for(int yIt = 0; yIt < 2; yIt++)
				for(int zIt = 0; zIt < 2; zIt++)
					if(!children[xIt][yIt][zIt]->IsLeaf()) {
						collapseChildren = false;
					}
					else if(sameValue == -1){
						sameValue = children[xIt][yIt][zIt]->GetValue(0,0,0);
					}
					else if(children[xIt][yIt][zIt]->GetValue(0,0,0) != sameValue){
						collapseChildren = false;
					}

		if(collapseChildren) {
			isLeaf = true;
			value = sameValue;

			for(int xIt = 0; xIt < 2; xIt++)
				for(int yIt = 0; yIt < 2; yIt++)
					for(int zIt = 0; zIt < 2; zIt++) {
						delete children[xIt][yIt][zIt];
						children[xIt][yIt][zIt] = 0;
					}
		}
	}
}

int MapOctree :: GetQuadrantX(int x) const {
	if(x >= x0 + (x1 - x0)/2) {
		return 1;
	}
	else{
		return 0;
	}
}

int MapOctree :: GetQuadrantY(int y) const {
	if(y >= y0 + (y1 - y0)/2) {
		return 1;
	}
	else{
		return 0;
	}
}

int MapOctree :: GetQuadrantZ(int z) const {
	if(z >= z0 + (z1 - z0)/2) {
		return 1;
	}
	else{
		return 0;
	}
}


int MapOctree :: GetValue(int x, int y, int z) const {
	if(isLeaf){
		return value;
	}
	else {
		children[GetQuadrantX(x)][GetQuadrantY(y)][GetQuadrantZ(z)]->GetValue(x, y, z);
	}
}





