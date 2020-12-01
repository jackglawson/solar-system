#include "functions.h"


bool is_in_cube(vector<double> centre, double width, vector<double> r) {
	double min_x = centre.at(0) - width / 2;
	double max_x = centre.at(0) + width / 2;
	double min_y = centre.at(1) - width / 2;
	double max_y = centre.at(1) + width / 2;
	double min_z = centre.at(2) - width / 2;
	double max_z = centre.at(2) + width / 2;

	if (r[0] < min_x || r[0] >= max_x ||
		r[1] < min_y || r[1] >= max_y ||
		r[2] < min_z || r[2] >= max_z) {
		return false;
	}
	else {
		return true;
	}
}


vector<vector<double>> cartesian_product(vector<double> xs, vector<double> ys, vector<double> zs) {
	vector<vector<double>> product;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++) {
				vector<double> new_perm{ xs[i], ys[j], zs[k] };
				product.push_back(new_perm);
			}
		}
	}
	return product;
}


double dist(vector<double> r1, vector<double> r2) {
	double dist{ 0 };
	for (int i{ 0 }; i < 3; i++) {
		dist += pow(r1[i] - r2[i], 2);
	}
	dist = sqrt(dist);
	return dist;
}


vector<double> acc(vector<double> r_test, vector<double> r, double m) {
	if (p::G == 0) {
		throw logic_error("param G has not been passed");
	}
	double d = dist(r_test, r);
	vector<double> a{ 0, 0, 0 };
	for (int i = 0; i < 3; i++) {
		double disp = r_test[i] - r[i];
		a[i] = -disp * p::G * m / pow(d, 3);
	}
	return a;
}


vector<double> find_com(vector<vector<double>> rs, vector<double> ms) {
	vector<double> com = rs[0];
	double cum_m = ms[0];

	for (int n{ 1 }; n < rs.size(); n++) {
		double new_cum_m = cum_m + ms[n];
		for (int i = 0; i < 3; i++) {
			com[i] *= cum_m;
			com[i] += ms[n] * rs[n][i];
			com[i] /= new_cum_m;
		}
		cum_m = new_cum_m;
	}

	return com;
}


vector<double> multiply_vector(vector<double> v, double s) {
	for (int i{ 0 }; i < v.size(); i++) {
		v[i] = v[i] * s;
	}
	return v;
}


vector<double> add_vectors(vector<double> v1, vector<double> v2) {
	assert(v1.size() == v2.size());
	for (int i{ 0 }; i < v1.size(); i++) {
		v1[i] += v2[i];
	}
	return v1;
}


vector<vector<double>> multiply_vector(vector<vector<double>> v, double s) {
	for (int i{ 0 }; i < v.size(); i++) {
		for (int j{ 0 }; j < v[0].size(); j++) {
			v[i][j] = v[i][j] * s;
		}
	}
	return v;
}


vector<vector<double>> add_vectors(vector<vector<double>> v1, vector<vector<double>> v2) {
	assert(v1.size() == v2.size());
	assert(v1[0].size() == v2[0].size());
	for (int i{ 0 }; i < v1.size(); i++) {
		for (int j{ 0 }; j < v1[0].size(); j++) {
			v1[i][j] += v2[i][j];
		}
	}
	return v1;
}


//vector<Particle> combine_overlapping_particles(vector<Particle> particles) {
//
//}
