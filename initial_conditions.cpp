#include "initial_conditions.h"


double random_uniform(double min, double max) {
	// Generate a uniformly distributed random double in the range [max, min)
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);
	return dis(gen);
}


vector<double> generate_r_uniform_sphere() {
	double radius = ICr::UniformSphere::max_r * cbrt(random_uniform(0, 1));
	double phi = random_uniform(0, M_PI);
	double theta = acos(1 - 2 * random_uniform(0, 1));

	double x = radius * sin(theta) * cos(phi);
	double y = radius * sin(theta) * sin(phi);
	double z = radius * cos(theta);

	return vector<double>{x, y, z};
}


vector<double> generate_r_uniform_disc() {
	double phi = random_uniform(0, M_PI);
	double radius = ICr::UniformDisc::max_r * sqrt(random_uniform(0, 1));

	double x = radius * cos(phi);
	double y = radius * sin(phi);
	double z = random_uniform(-ICr::UniformDisc::width / 2, ICr::UniformDisc::width / 2);

	return vector<double>{ x, y, z };
}


vector<double> generate_r() {
	switch (p::icr_type) {
	case 0:
		return generate_r_uniform_sphere();
	case 1:
		return generate_r_uniform_disc();
	}
}


vector<double> generate_random_v_vector() {
	double magnitude = p::max_random_v * cbrt(random_uniform(0, 1));
	double phi = random_uniform(0, M_PI);
	double theta = acos(1 - 2 * random_uniform(0, 1));

	double v_x = magnitude * sin(theta) * cos(phi);
	double v_y = magnitude * sin(theta) * sin(phi);
	double v_z = magnitude * cos(theta);

	return vector<double>{v_x, v_y, v_z};
}


vector<double> generate_v_orbit_central_mass(vector<double> r) {
	double rho = sqrt(pow(r[0], 2) + pow(r[1], 2));
	double theta = atan2(r[0], r[1]);
	double v = sqrt(p::G * ICv::orbit_central_mass::central_mass / rho);
	return vector<double> {-v * cos(theta), v * sin(theta), 0};
}


vector<double> generate_v(vector<double> r) {
	switch (p::icv_type) {
	case 0:
		return generate_random_v_vector();
	case 1:
		return generate_v_orbit_central_mass(r);
	}
}


double generate_m() {
	return random_uniform(p::min_m, p::max_m);
}


vector<Particle*> generate_ics() {
	int n = p::num_particles - p::extra_particles.size();
	vector<Particle*> particles(n, nullptr);


	for (int i = 0; i < n; i++) {
		vector<double> r{ generate_r() };
		vector<double> v{ generate_v(r) };
		double m{ generate_m() };
		Particle* p = new Particle{ r, v, m, p::radius_type };
		particles[i] = p;
	}


	for (int i = 0; i < p::extra_particles.size(); i++) {
		particles.push_back(&p::extra_particles[i]);
	}

	return particles;
}
