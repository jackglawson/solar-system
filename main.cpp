#include "sim.h"
#include "collision.h"


int main() {
	vector<Particle> particles;
	particles.push_back(Particle(vector<double>{0, 0, 0}, vector<double>{0, 0, 0}, 1, 0));
	particles.push_back(Particle(vector<double>{0.05, 0, 0}, vector<double>{0, 0, 0}, 1, 0));

	particles = do_collisions(particles);

	for (int i = 0; i < particles.size(); i++) {
		particles[i].print();
	}

	//Simulation sim = run_sim();
	//sim.save();
	//sim.print();
	//return 0;
}