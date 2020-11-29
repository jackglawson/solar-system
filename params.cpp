#include "params.h"


namespace ICr {
	// Initial condition options for the positions of particles
	// 0 ... uniform sphere
	// 1 ... uniform disc

	namespace UniformSphere {
		double max_r = 10;
	};


	namespace UniformDisc {
		double max_r = 10;
		double width = 0;
	};
}


namespace ICv {
	// Initial condition options for the velocities of particles
	// 0 ... just random velocities
	// 1 ... all particles orbit a single central mass

	namespace just_random {
		;
	}

	namespace orbit_central_mass {
		double central_mass = 1000;
	}
}


namespace RadiiFunctions {
	// radius_types:
	// 0 ... fixed radius
	// 1 ... constant density radius

	namespace Fixed {
		// The radius does not depend on the mass of the particle
		double radius = 0.1;
	};


	namespace ConstDensity {
		// The radius increases with the cube root of mass
		double radius_of_unit_mass_particle = 0.1;

		double get_radius(double m) {
			assert(m > 0, "Invalid mass");
			return radius_of_unit_mass_particle * cbrt(m);
		}
	}
}


namespace p {
	// Initial condition parameters
	int num_particles = 1000;		// including extra particles
	double min_m = 0.01;
	double max_m = 0.2;
	int icr_type = 1;				// 0 ... uniform sphere
									// 1 ... uniform disc

	int icv_type = 1;				// 0 ... just random velocities
									// 1 ... all particles orbit a single central mass

	double min_random_v = 0;		// add a random vector to the velocity
	double max_random_v = 0;

	int radius_type = 1;			// 0 ... fixed radius
									// 1 ... constant density radius
	
	// Simulation parameters
	double tot_t = 10;
	double dt = 0.05;				// time between frames
	double G = 1;
	bool gravity_on = true;
	bool collisions_on = true;

	// Extra particles
	vector<Particle> extra_particles{ Particle(vector<double>{0,0,0}, vector<double>{0,0,0}, 1000, 0) };

	// Integration settings
	double acc = 0.001;				// desired truncation error per step. The truncation error is absolute. 
	double S = 10.0;				// step-length cannot change by more than this factor from step to step
	int maxrept = 10;				// maximum allowable number of step recalculations
	double h_start = 0.0001;		// length of first step.
	double h_warn = 0.00001;		// warn the user if step-length falls below this
	double h_min = 0.0;				// minimum allowable step-length
	double h_max = 1.0;				// maximum allowable step-length
	double h_drop_factor = 0.7;		// if the truncation error of a step is unacceptable, multiply h by this before re-trying


	// Collision settings
	int max_collision_checks = 1000;	// once this many collisions have been detected in a single step, the program will exit

	// Octree settings
	double qt = 1;

	// File settings
	string sim_out = "simulation.dat";
}
