#pragma once

#include <vector>
#include <assert.h>
#include "Particle.h"
class Particle;			// Need this because of circular include
using namespace std;


namespace ICs {

	namespace UniformSphere {
		extern double max_r;
	};


	namespace UniformDisc {
		extern double max_r;
		extern double width;
	};
}


namespace RadiiFunctions {

	namespace Fixed {
		extern double radius;
	};

	namespace ConstDensity {
		extern double radius_of_unit_mass_particle;
		double get_radius(double m);
	}
}


namespace p {
	// Initial condition parameters
	extern int num_particles;				
	extern double min_m;
	extern double max_m;
	extern int ic_type;

	// Initial condition adjustment parameters
	extern int ica_type;

	// Simulation parameters
	extern double tot_t;
	extern double dt;
	extern double G;
	extern bool gravity_on;
	extern bool collisions_on;

	// Extra particles
	extern vector<Particle> extra_particles;

	// Integration settings
	extern double acc;
	extern double S;
	extern int maxrept;
	extern double h_min;
	extern double h_max;
	extern int flag;
									
	// Octree settings
	extern double qt;
}
