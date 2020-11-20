#include "Particle.h"


Particle::Particle() {
	r = vector<double>{ 0,0,0 };
	v = vector<double>{ 0,0,0 };
	m = 0;
	radius_type = 0;
}


Particle::Particle(vector<double> r_in, vector<double> v_in, double m_in, int radius_type_in) {
	r = r_in;
	v = v_in;
	m = m_in;
	radius_type = radius_type_in;
}


Particle::Particle(Particle p1, Particle p2) {
	// Two particle coalesce
	double m1 = p1.get_m();
	double m2 = p2.get_m();
	m = m1 + m2;

	vector<double> r1 = p1.get_r();
	vector<double> r2 = p2.get_r();
	vector<double> r(3);
	vector<double> v1 = p1.get_v();
	vector<double> v2 = p2.get_v();
	vector<double> v(3);
	for (int i = 0; i < 3; i++) {
		r[i] = (m1 * r1[i] + m2 * r2[i]) / m;
		v[i] = (m1 * v1[i] + m2 * v2[i]) / m;
	}

	if (p1.get_radius_type() == 0 or p2.get_radius_type() == 0) {
		radius_type = 0;
	}
	else if (p1.get_radius_type() == p2.get_radius_type()) {
		radius_type = p1.get_radius_type();
	}
	else {
		cout << "Case not handled yet";
		exit(1);
	}
}


vector<double> Particle::get_r() {
	return r;
}

vector<double> Particle::get_v() {
	return v;
}

double Particle::get_m() {
	return m;
}

int Particle::get_radius_type() {
	return radius_type;
}

double Particle::get_radius() {
	switch (radius_type) {
	case 0:
		return RadiiFunctions::Fixed::radius;
	case 1:
		return RadiiFunctions::ConstDensity::get_radius(m);
	}
}

void Particle::set_r(vector<double> r_in) {
	r = r_in;
}

void Particle::set_v(vector<double> v_in) {
	v = v_in;
}

void Particle::print() {
	cout << "Particle:\n";
	cout << "\t" << "r = " << r[0] << ", " << r[1] << ", " << r[2] << "\n";
	cout << "\t" << "v = " << v[0] << ", " << v[1] << ", " << v[2] << "\n";
	cout << "\t" << "m = " << m << "\n";
}