#include "initial_conditions.h"


double random_uniform(double min, double max) {
	// Generate a uniformly distributed random double in the range [max, min)
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);
	return dis(gen);
}


vector<double> generate_r_uniform_sphere() {
	double radius = ICs::UniformSphere::max_r * cbrt(random_uniform(0, 1));
	double phi = random_uniform(0, M_PI);
	double theta = acos(1 - 2 * random_uniform(0, 1));

	double x = radius * sin(theta) * cos(phi);
	double y = radius * sin(theta) * sin(phi);
	double z = radius * cos(theta);

	return vector<double>{x, y, z};
}


vector<double> generate_r_uniform_disc() {
	double phi = random_uniform(0, M_PI);
	double radius = ICs::UniformDisc::max_r * sqrt(random_uniform(0, 1));

	double x = radius * cos(phi);
	double y = radius * sin(phi);
	double z = random_uniform(-ICs::UniformDisc::width / 2, ICs::UniformDisc::width / 2);

	return vector<double>{ x, y, z };
}


vector<double> generate_r() {
	switch (p::ic_type) {
	case 0:
		return generate_r_uniform_sphere();
	case 1:
		return generate_r_uniform_disc();
	}
}


vector<double> 


vector<Particle> generate_ics() {
	vector<Particle> particles;
	int n = p::num_particles - p::extra_particles.size();

	for (int i = 0; i < n; i++) {
		


		particles.push_back(Particle(generate_r(), ))
	}

}
