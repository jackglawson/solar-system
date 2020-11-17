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


vector<double> Particle::get_r() {
	return r;
}

vector<double> Particle::get_v() {
	return v;
}

double Particle::get_m() {
	return m;
}

double Particle::get_radius() {
	if (radius_type == 0) {
		return RadiiFunctions::Fixed::radius;
	}
	else if (radius_type == 1) {
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