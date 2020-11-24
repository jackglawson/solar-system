#include "sim.h"
#include "collision.h"

// All this for detecting memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif


int main() {
	// Try making a deconstructor method for the particle class?


	vector<Particle> particles;
	particles.push_back(Particle(vector<double>{0, 0, 0}, vector<double>{0, 0, 0}, 1000, 0));
	particles.push_back(Particle(vector<double>{5, 0, 0}, vector<double>{0, 20, 0}, 1, 0));	

	Simulation sim = run_sim(particles);
	sim.save();
	// sim.print();
	_CrtDumpMemoryLeaks();
	return 0;
}