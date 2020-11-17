#include "params.h"


namespace ICs {

	namespace UniformSphere {
		double max_r = 10;
	};


	namespace UniformDisc {
		double max_r = 10;
		double width = 1;
	};
}


namespace RadiiFunctions {
	// radius_types:
	// 0 ... fixed radius
	// 1 ... constant density radius

	namespace Fixed {
		// The radius does not depend on the mass of the particle
		double radius = 1;
	};


	namespace ConstDensity {
		// The radius increases with the cube root of mass
		double radius_of_unit_mass_particle = 1;

		double get_radius(double m) {
			assert(m > 0, "Invalid mass");
			return radius_of_unit_mass_particle * cbrt(m);
		}
	}
}


namespace p {
	// Initial condition parameters
	int num_particles = 10;			// including extra particles
	double min_m = 0.1;
	double max_m = 10;
	int ic_type = 0;				// 0 ... uniform sphere
									// 1 ... uniform disc

	// Initial condition adjustment parameters
	int ica_type = 0;				// 0 ... no adjustment
									// 1 ... all particles orbit a single central mass
	
	// Simulation parameters
	double tot_t = 5;
	double dt = 1;				// time between frames
	double G = 1;
	bool gravity_on = true;
	bool collisions_on = true;

	// Extra particles
	vector<Particle> extra_particles{};

	// Integration settings
	double acc = 0.001;			// desired truncation error per step
	double S = 10;					// step-length cannot change by more than this factor from step to step
	int maxrept = 10;				// maximum allowable number of step recalculations
	double h_min = 0.001;			// minimum allowable step-length
	double h_max = 1;				// maximum allowable step-length
	int flag = 2;					// controls manner in which truncation error is calculated
									// flag = 0 ... error is absolute
									// flag = 1 ... error is relative
									// flag = 2 ... error is mixed

	// Octree settings
	double qt = 1;
}
