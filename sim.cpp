#include "sim.h"


Simulation::Simulation(vector<Particle> particles) {
	timestamps = vector<double>{ 0 };
	snapshots = vector<vector<Particle>>{ particles };
}


void Simulation::add_snapshot(double t, vector<Particle> particles) {
	timestamps.push_back(t);
	snapshots.push_back(particles);
}


void Simulation::print() {
	cout << "SIMULATION RESULT:\n\n";
	for (int i = 0; i < timestamps.size(); i++) {
		cout << "t = " << timestamps[i] << "\n";
		for (int j = 0; j < snapshots[i].size(); j++) {
			snapshots[i][j].print();
		}
		cout << "\n";
	}
}


Simulation run_sim() {

	vector<Particle> particles;
	particles.push_back(Particle(vector<double>{0, 0, 0}, vector<double>{0, 0, 0}, 1, 0));
	particles.push_back(Particle(vector<double>{1, 0, 0}, vector<double>{0, 0, 0}, 1, 0));


	Simulation simulation{particles};


	double t = 0;
	while (t < p::tot_t) {
		double next_t = t + p::dt;
		particles = walk_to(t, next_t, particles);
		simulation.add_snapshot(next_t, particles);
		t = next_t;
	}

	return simulation;
}
