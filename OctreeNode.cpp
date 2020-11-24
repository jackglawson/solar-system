#include "OctreeNode.h"


OctreeNode::OctreeNode(vector<double> centre, double width, vector<vector<double>> rs, vector<double> ms) {
	c = centre;
	w = width;
	num_particles = 0;
	total_m = 0;

	vector<double> r;
	vector<vector<double>> selected_rs;
	vector<double> selected_ms;
	for (int i{ 0 }; i < rs.size(); i++) {
		r = rs.at(i);
		if (is_in_cube(c, w, r)) {
			selected_rs.push_back(r);
			selected_ms.push_back(ms.at(i));
			num_particles += 1;
			total_m += ms.at(i);
		}
	}

	if (num_particles > 0) {
		com = find_com(selected_rs, selected_ms);
	}
	else {
		com = vector<double>{ 0,0,0 };
	}
	
	if (num_particles <= 1) {
		children = vector<OctreeNode*>{ NULL };
		has_children = false;
	}
	else {
		double new_width = w / 2;
		vector<double> new_xs{ centre[0] - w / 4, centre[0] + w / 4 };
		vector<double> new_ys{ centre[1] - w / 4, centre[1] + w / 4 };
		vector<double> new_zs{ centre[2] - w / 4, centre[2] + w / 4 };
		vector<vector<double>> new_centres = cartesian_product(new_xs, new_ys, new_zs);
		for (int i{ 0 }; i < 8; i++) {
			children.push_back(new OctreeNode(new_centres[i], new_width, selected_rs, selected_ms));
		}
		has_children = true;
	}
}

OctreeNode::~OctreeNode() {
	if (has_children) {
		for (int i = 0; i < 8; i++) {
			delete children[i];
		}
	}
}


void OctreeNode::print() {
	cout << "Node at ";
	for (int i{ 0 }; i < 3; i++) { cout << c[i] << ","; }
	cout << " with width " << w << " and " << num_particles << " particles\n";
}


void OctreeNode::print_children() {
	int num_children = children.size();
	if (!has_children) {
		cout << "No children";
	}
	else {
		cout << num_children << " children:\n";
		for (int i{ 0 }; i < num_children; i++) {
			children[i]->print();
		}
		cout << "\n";
	}
}