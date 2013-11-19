#ifndef MAP_OCTREE_H
#define MAP_OCTREE_H

class MapOctree{
public:
	MapOctree(int setX0, int setY0, int setZ0, int setX1, int setY1, int setZ1, int setValue);
	~MapOctree();

	void SetValue(int x, int y, int z, int setVal);
	int GetValue(int x, int y, int z) const;

	bool IsLeaf() const {return isLeaf;}

	static int numOfMapOctrees;

protected:

	int GetQuadrantX(int x) const;
	int GetQuadrantY(int y) const;
	int GetQuadrantZ(int z) const;

	bool isLeaf;
	int x0, y0, z0;
	int x1, y1, z1;
	int value;
	MapOctree* children[2][2][2];


};

#endif
