#include "sim.h"


void create_out_file() {
	ofstream outf{ p::sim_out };

	if (!outf)
	{
		std::cerr << p::sim_out << " could not be opened for writing!" << std::endl;
		exit(1);
	}

	outf << "timestamp rx ry rz m radius" << "\n";
}


void write_new_snapshot(double timestamp, vector<Particle*> snapshot){
	ofstream outf;
	outf.open(p::sim_out, ios::app);

	if (!outf)
	{
		std::cerr << p::sim_out << " could not be opened for writing!" << std::endl;
		exit(1);
	}

	for (int i = 0; i < snapshot.size(); i++) {
		outf << timestamp << " ";
		Particle* particle = snapshot[i];
		vector<double> r = particle->get_r();
		outf << r[0] << " ";
		outf << r[1] << " ";
		outf << r[2] << " ";
		outf << particle->get_m() << " ";
		outf << particle->get_radius() << " ";
		outf << "\n";
	}
}


void run_sim(vector<Particle*> particles) {

	if (particles.size() == 0) {
		particles = generate_ics();
	}

	double t = 0;
	create_out_file();
	write_new_snapshot(t, particles);

	while (t < p::tot_t) {
		double next_t = t + p::dt;
		cout << "Walking to t=" << next_t << "\n";
		particles = walk_to(t, next_t, particles);
		write_new_snapshot(next_t, particles);
		t = next_t;
	}
}
