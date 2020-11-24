#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <vector>
#include <assert.h>
#include "Particle.h"
class Particle;			// Need this because of circular include
using namespace std;


namespace ICr {

	namespace UniformSphere {
		extern double max_r;
	};


	namespace UniformDisc {
		extern double max_r;
		extern double width;
	};
}


namespace ICv {

	namespace no_velocity {
		;
	}

	namespace orbit_central_mass {
		extern double central_mass;
	}
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
	extern int icr_type;
	extern int icv_type;
	extern double min_random_v;
	extern double max_random_v;
	extern 	int radius_type;

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

	// Collision settings
	extern int max_collision_checks;
									
	// Octree settings
	extern double qt;
}
