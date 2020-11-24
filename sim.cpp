#include "sim.h"


Simulation::Simulation(vector<shared_ptr<Particle>> particles) {
	timestamps = vector<double>{ 0 };
	//snapshots = vector<unique_ptr<Particle>>{ particles };
}


//void Simulation::add_snapshot(double t, vector<Particle> particles) {
//	timestamps.push_back(t);
//	snapshots.push_back(particles);
//}
//
//
//void Simulation::print() {
//	cout << "SIMULATION RESULT:\n\n";
//	for (int t = 0; t < timestamps.size(); t++) {
//		cout << "t = " << timestamps[t] << "\n";
//		for (int i = 0; i < snapshots[t].size(); i++) {
//			snapshots[t][i].print();
//		}
//		cout << "\n";
//	}
//}
//
//
//void Simulation::save(string filename) {
//	// save to a .dat file
//
//	ofstream outf{ filename };
//
//	if (!outf)
//	{
//		std::cerr << filename << " could not be opened for writing!" << std::endl;
//		exit(1);
//	}
//
//	cout << "Saving to " << filename << "\n";
//
//	outf << "timestamp rx ry rz m radius" << "\n";
//	for (int t = 0; t < timestamps.size(); t++) {
//		for (int i = 0; i < snapshots[t].size(); i++) {
//			outf << timestamps[t] << " ";
//			Particle particle = snapshots[t][i];
//			vector<double> r = particle.get_r();
//			outf << r[0] << " ";
//			outf << r[1] << " ";
//			outf << r[2] << " ";
//			outf << particle.get_m() << " ";
//			outf << particle.get_radius() << " ";
//			outf << "\n";
//		}
//	}
//
//	cout << "Saved\n";
//}


Simulation run_sim(vector<shared_ptr<Particle>> particles) {

	//if (particles.size() == 0) {
	//	particles = generate_ics();
	//}

	// Simulation simulation{particles};

	double t = 0;
	while (t < p::tot_t) {
		double next_t = t + p::dt;
		cout << "Walking to t=" << next_t << "\n";
		particles = walk_to(t, next_t, particles);
		// simulation.add_snapshot(next_t, particles); // not the leak
		t = next_t;
	}

	return Simulation(particles);
	// return simulation;
}
