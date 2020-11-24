#include "sim.h"
#include "collision.h"


int main() {
	vector<Particle> particles;
	particles.push_back(Particle(vector<double>{0, 0, 0}, vector<double>{0, 0, 0}, 1, 0));
	particles.push_back(Particle(vector<double>{5, 0, 0}, vector<double>{0, 0, 0}, 1, 0));	

	Simulation sim = run_sim(particles);
	sim.save();
	sim.print();
	return 0;
}