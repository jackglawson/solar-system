#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif

#include "functions.h"

class OctreeNode {
private:
	vector<double> c;
	double w;
	int num_particles;
	double total_m;
	vector<double> com;
	bool has_children;
	vector<OctreeNode*> children;

public:
	OctreeNode();
	OctreeNode(vector<double> centre, double width, vector<vector<double>> rs, vector<double> ms);

	~OctreeNode();
	OctreeNode(const OctreeNode& that) = delete;
	OctreeNode operator=(const OctreeNode& that) = delete;

	void print();
	void print_children();

	friend class Octree;
};