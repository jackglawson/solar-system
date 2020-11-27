#include "Octree.h"
#include <algorithm>


Octree::Octree() {
	vector<double> centre{ 0,0,0 };
	double width{ 0 };
	vector<vector<double>> rs{};
	vector<double> ms{};
	root = new OctreeNode(centre, width, rs, ms);
}


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


Octree::Octree(vector<Particle> particles) {
	// pass by reference?
	int n = particles.size();
	vector<vector<double>> rs(n, vector<double>(3));
	vector<double> ms(n);
	for (int i{ 0 }; i < n; i++) {
		rs[i] = particles[i].get_r();
		ms[i] = particles[i].get_m();
	}
	Octree::Octree(rs, ms);
}


Octree::~Octree() {
	delete root;
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


vector<vector<double>> Octree::find_all_accs(vector<vector<double>> rs, vector<double> ms, vector<double> radii) {
	assert(rs.size() == ms.size());
	assert(rs.size() == radii.size());
	int n = rs.size();
	vector<vector<double>> all_accs(n);
	for (int i = 0; i < n; i++) {
		all_accs[i] = Octree::find_acc(rs[i], ms[i], radii[i]);
	}
	return all_accs;
}