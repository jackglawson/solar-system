#include "Octree.h"
#include <algorithm>

Octree::Octree(vector<vector<double>> rs, vector<double> ms) {
	vector<double> centre = find_com(rs, ms);
	double max_coord{ 0 };
	for (int n{ 0 }; n < rs.size(); n++) {
		for (int i{ 0 }; i < 3; i++) {
			max_coord = max(max_coord, abs(rs[n][i]));
		}
	}
	double width = max_coord * 2 + 1.0;
	root = new OctreeNode(centre, width, rs, ms);
}


vector<double> Octree::find_acc(vector<double> r, double m, double radius) {
	if (p::qt == 0) {
		throw logic_error("param qt has not been passed");
	}

	vector<double> a{ 0, 0, 0 };

	stack<OctreeNode*> stack;
	stack.push(root);
	while (!stack.empty()) {
		OctreeNode* v = stack.top();
		stack.pop();
		if (v->num_particles == 0) {
			continue;
		}
		if (v->num_particles == 1 && dist(v->com, r) < radius) {
			continue; // don't want a particle to exert a force on itself
		}

		double quotient = v->w / dist(v->com, r);
		if (quotient <= p::qt || v->num_particles == 1) {
			vector<double> new_a = acc(r, v->com, v->total_m);
			for (int i{ 0 }; i < 3; i++) {
				a[i] += new_a[i];
			}
		}
		else {
			for (int i = 0; i < 8; i++) {
				stack.push(v->children[i]);
			}
		}
	}
	return a;
}