#include "sim.h"


int main() {
	Simulation sim = run_sim();
	sim.save();
	// sim.print();
	return 0;
}