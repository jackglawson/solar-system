#pragma once
#include "OctreeNode.h"

class Octree {
public:
	OctreeNode* root;

public:
	Octree(vector<vector<double>> rs, vector<double> ms);

	vector<double> find_acc(vector<double> r, double m, double radius);
};
