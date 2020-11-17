#pragma once
#include "OctreeNode.h"
#include "Particle.h"

class Octree {
public:
	OctreeNode* root;

public:
	Octree();
	Octree(vector<vector<double>> rs, vector<double> ms);
	Octree(vector<Particle>);
	
	vector<double> find_acc(vector<double> r, double m, double radius);
	vector<vector<double>> find_all_accs(vector<vector<double>> rs, vector<double> ms, vector<double> radii);
};
