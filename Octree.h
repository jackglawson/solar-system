#pragma once
#include "OctreeNode.h"
#include "Particle.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

class Octree {
public:
	OctreeNode* root;

public:
	Octree();
	Octree(vector<vector<double>> rs, vector<double> ms);
	Octree(vector<Particle>);
	
	~Octree();
	Octree(const Octree& that) = delete;
	Octree operator=(const Octree& that) = delete;
	
	vector<double> find_acc(vector<double> r, double m, double radius);
	vector<vector<double>> find_all_accs(vector<vector<double>> rs, vector<double> ms, vector<double> radii);
};
